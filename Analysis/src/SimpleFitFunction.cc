#include "../interface/SimpleFitFunction.hh"
#include "CMSAnalysis/Analysis/interface/ExpressionFitFunction.hh"
#include "CMSAnalysis/Analysis/interface/DSCBFitFunction.hh"
#include "CMSAnalysis/Analysis/interface/PowerLawFitFunction.hh"
#include "CMSAnalysis/Analysis/interface/DoubleGaussianFitFunction.hh"
#include "CMSAnalysis/Analysis/interface/GausLogPowerNormFitFunction.hh"
#include "CMSAnalysis/Analysis/interface/VoigtFitFunction.hh"
#include "TF1.h"
#include "TBuffer.h"
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

ClassImp(SimpleFitFunction);

void SimpleFitFunction::Streamer(TBuffer &buffer)
{
    if (!buffer.IsReading())
    {
        buffer.WriteClassBuffer(SimpleFitFunction::Class(), this);
        return;
    }
    buffer.ReadClassBuffer(SimpleFitFunction::Class(), this);
    const auto restore = [this](TF1 &tf1) {
        // must note here that for TF1 the reader registers are global
        // cannot restore callback
        // so we dont use the global ROOT list
        
        tf1.AddToGlobalList(false);
        restoreFunction(tf1);
    };
    restore(function);
    for (auto &[name, variations] : systematics)
    {
        restore(variations.first);
        restore(variations.second);
    }
}


SimpleFitFunction::SimpleFitFunction(const TF1 &func, FunctionType funcType)
    : FitFunction(funcType, func.GetName()), function(func)
{
}

SimpleFitFunction::SimpleFitFunction(FunctionType funcType)
    : FitFunction(funcType, "")
{
}

TF1 *SimpleFitFunction::getFunction()
{
    return &function;
}

const TF1 *SimpleFitFunction::getFunction() const
{
    return &function;
}

void SimpleFitFunction::setFunction(const TF1 &func, FunctionType funcType)
{
    if (funcType != getFunctionType())
        throw std::invalid_argument("Cannot change a SimpleFitFunction's type");
    function = func;
    setName(func.GetName());
}

double SimpleFitFunction::getMin() const
{
    double min;
    double max;
    function.GetRange(min, max);
    return min;
}
double SimpleFitFunction::getMax() const
{
    double min;
    double max;
    function.GetRange(min, max);
    return max;
}

std::string SimpleFitFunction::getParameter(std::string name) const
{
    auto parameters = decodeName(getName());
    auto parameter = parameters.find(name);
    return parameter == parameters.end() ? "" : parameter->second;
}


std::shared_ptr<SimpleFitFunction> SimpleFitFunction::createFunctionOfType(FunctionType functionType,
    const std::string &name, const std::string &expFormula, double min, double max)
{
    switch (functionType)
    {
    case FunctionType::ExpressionFormula:
        return std::make_shared<ExpressionFitFunction>(name, expFormula, min, max);
    case FunctionType::DoubleSidedCrystalBall:
        return std::make_shared<DSCBFitFunction>(name, min, max);
    case FunctionType::PowerLaw:
        return std::make_shared<PowerLawFitFunction>(name, min, max);
    case FunctionType::DoubleGaussian:
        return std::make_shared<DoubleGaussianFitFunction>(name, min, max);
    case FunctionType::GausLogPowerNorm:
        return std::make_shared<GausLogPowerNormFitFunction>(name, min, max);
    case FunctionType::Voigt:
        return std::make_shared<VoigtFitFunction>(name, min, max);
    }
    throw std::invalid_argument("Unknown FitFunction type");
}

double SimpleFitFunction::evaluate(double x) const
{
    return function.Eval(x);
}

double SimpleFitFunction::evaluate(double x, const NuisanceValues &nuisances) const
{
    // new hierarchy for the function with simplefitfunc

    std::vector<double> parameters(function.GetNpar());
    function.GetParameters(parameters.data());
    for (const auto &[name, delta] : nuisances)
    {
        if (!std::isfinite(delta))
            throw std::invalid_argument("Shape-systematic deltas must be finite");
        if (delta == 0)
            continue;
        const TF1 *up = getSystematic(name, true);
        const TF1 *down = getSystematic(name, false);
        // nusiance only effects some rows
       
        if (!up && !down)
            continue;
        if (!up || !down || up->GetNpar() != function.GetNpar() || down->GetNpar() != function.GetNpar())
            throw std::invalid_argument("Shape variations must match the nominal parameter count");
        for (int p = 0; p < function.GetNpar(); ++p)
        {
            const double nominal = function.GetParameter(p);
            if (!std::isfinite(nominal) || !std::isfinite(up->GetParameter(p)) || !std::isfinite(down->GetParameter(p)))
                throw std::invalid_argument("Shape-systematic parameters must be finite");
            if (!variesWithSystematic(p))
                continue;
            const double upShift = up->GetParameter(p) - nominal;
            const double downShift = down->GetParameter(p) - nominal;
            if (!std::isfinite(upShift) || !std::isfinite(downShift))
                throw std::invalid_argument("Shape-systematic shifts must be finite");
            parameters[p] += std::abs(delta) * (delta >= 0 ? upShift : downShift);
        }
    }
    return evaluateWithParameters(x, parameters);
}

// change depending on type
double SimpleFitFunction::evaluate(double observable, double, const NuisanceValues &nuisances) const
{
    return evaluate(observable, nuisances);
}

double SimpleFitFunction::evaluateWithParameters(double x, const std::vector<double> &parameters) const
{
    if (parameters.size() != static_cast<size_t>(function.GetNpar()))
        throw std::invalid_argument("FitFunction parameter count does not match its TF1");
    return function.EvalPar(&x, parameters.data());
}

std::string SimpleFitFunction::getExpression(const std::string &variable) const
{
    //we assume power law paramter function
    std::ostringstream expression;
    expression << function.GetParameter(0) << " * (" << variable << " - "
               << function.GetParameter(1) << ")^" << function.GetParameter(2);
    return expression.str();
}

std::string SimpleFitFunction::getNormExpression(const std::string &) const
{
    std::ostringstream expression;
    expression << std::setprecision(17) << function.Integral(getMin(), getMax());
    return expression.str();
}

// Helper function for splitting strings
std::vector<std::string> SimpleFitFunction::split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

// OLD CODE
std::ostream &operator<<(std::ostream &stream, const SimpleFitFunction &function)
{
    const TF1 *func = function.getFunction();
    // std::cout << "Got functions\n";
    stream << "Name: " << func->GetName() << '\n';
    stream << "FunctionTypeEnum: " << static_cast<int>(function.getFunctionType()) << '\n';
    stream << "ExpressionFormula: ";
    if (function.getFunctionType() == FitFunction::FunctionType::ExpressionFormula)
    {
        stream << func->GetExpFormula() << '\n';
    }
    else
    {
        stream << "None\n";
    }

    // std::cout << "Got expFormula\n";
    // auto formulaName = FitFunction::getFormulaName(func->GetName());
    // auto it = std::find(FitFunction::functionList.begin(), FitFunction::functionList.end(), formulaName);
    // if (it != FitFunction::functionList.end())
    // {
    // 	stream << "Name: " << func->GetName() << '\n';
    // 	stream << "Function: " << formulaName << '\n';
    // }
    // else
    // {
    // 	stream << "Name: " << func->GetName() << '\n';
    // 	stream << "Function: " << func->GetExpFormula() << '\n';
    // }
    double min = 0;
    double max = 0;
    func->GetRange(min, max);
    stream << "Range: " << min << ' ' << max << '\n';
    stream << "NumOfParameters: " << func->GetNpar() << '\n';

    stream << "ParaNames: ";
    for (int i = 0; i < func->GetNpar(); ++i)
    {
        stream << func->GetParName(i) << ' ';
        // stream << 1 << ' ';
    }

    stream << '\n' << "Parameters: ";
    for (int i = 0; i < func->GetNpar(); ++i)
    {
        stream << func->GetParameter(i) << ' ';
        // stream << 1 << ' ';
    }

    stream << '\n' << "ParamErrors: ";

    for (int i = 0; i < func->GetNpar(); ++i)
    {
        stream << func->GetParError(i) << ' ';
        // stream << 1 << ' ';
    }

    stream << '\n';
    // std::cout << "Got parameters\n";

    // --- Systematics Section ---
    auto systematics = function.listSystematics();
    if (!systematics.empty())
    {
        stream << "Systematics: " << systematics.size() << '\n';
        for (const auto &sysName : systematics)
        {
            const auto *const upFunc = function.getSystematic(sysName, true);
            const auto *const downFunc = function.getSystematic(sysName, false);
            stream << "  Systematic: " << sysName << '\n';

            if (upFunc)
            {
                stream << "    UpParameters: ";
                for (int i = 0; i < upFunc->GetNpar(); ++i)
                {
                    stream << upFunc->GetParameter(i) << ' ';
                }
                stream << '\n';
            }

            if (downFunc)
            {
                stream << "    DownParameters: ";
                for (int i = 0; i < downFunc->GetNpar(); ++i)
                {
                    stream << downFunc->GetParameter(i) << ' ';
                }
                stream << '\n';
            }
        }
    }
    else
    {
        stream << "Systematics: 0\n";
    }

    return stream;
}

std::istream &operator>>(std::istream &stream, std::shared_ptr<SimpleFitFunction> &func)
{
    std::string line;
    std::string name;
    auto funcType = FitFunction::FunctionType(0);
    std::string expFormula;
    double min = 0.0, max = 0.0;
    int params = 0;
    int tempFuncType = 0;

    // --- Helper lambda to trim spaces ---
    auto trim = [](std::string &s) {
        s.erase(0, s.find_first_not_of(" \t"));
        s.erase(s.find_last_not_of(" \t") + 1);
    };



    // --- Read "Name:" line ---
    do
    {
        std::getline(stream, line);
    }
    while (stream && line.empty());
    if (!stream)
    {
        return stream;
    }
    // std::cout << "Next line " << line << "\n";
    if (line.find("Name:") != std::string::npos)
    {
        name = line.substr(5);
    }
    else
    {
        throw std::runtime_error ("Name not found");
    }
    trim(name);
    std::cout << "Reading function: " << name << '\n';

    // --- Read "FunctionTypeEnum:" line ---
    std::getline(stream, line);
    if (line.find("FunctionTypeEnum:") != std::string::npos)
    {
        tempFuncType = std::stoi(line.substr(17));
    }
    funcType = static_cast<FitFunction::FunctionType>(tempFuncType);

    // --- Read "ExpressionFormula:" line ---
    std::getline(stream, line);
    if (line.find("ExpressionFormula:") != std::string::npos)
    {
        expFormula = line.substr(18);
    }
    trim(expFormula);

    // --- Read "Range:" line ---
    std::getline(stream, line);
    if (line.find("Range:") != std::string::npos)
    {
        std::istringstream rangeStream(line.substr(6));
        rangeStream >> min >> max;
    }

    // --- Read "NumOfParameters:" line ---
    std::getline(stream, line);
    if (line.find("NumOfParameters:") != std::string::npos)
    {
        params = std::stoi(line.substr(16));
    }
    if (params <= 0 || params > 1000)
    {
        std::cerr << "Error: invalid NumOfParameters = " << params << " for " << name << '\n';
        return stream;
    }

    // --- Prepare containers ---
    std::vector<std::string> paramNames(params);
    std::vector<double> paramValues(params);
    std::vector<double> paramErrors(params);

    // --- Read "ParaNames:" line ---
    std::getline(stream, line);
    if (line.find("ParaNames:") != std::string::npos)
    {
        std::istringstream ss(line.substr(10));
        for (int i = 0; i < params && ss; ++i)
        {
            ss >> paramNames[i];
        }
    }

    // --- Read "Parameters:" line ---
    std::getline(stream, line);
    if (line.find("Parameters:") != std::string::npos)
    {
        std::istringstream ss(line.substr(11));
        for (int i = 0; i < params && ss; ++i)
        {
            ss >> paramValues[i];
        }
    }

    // --- Read "ParamErrors:" line ---
    std::getline(stream, line);
    if (line.find("ParamErrors:") != std::string::npos)
    {
        std::istringstream ss(line.substr(12));
        for (int i = 0; i < params && ss; ++i)
        {
            ss >> paramErrors[i];
        }
    }

    // --- Create FitFunction object ---
    auto function = SimpleFitFunction::createFunctionOfType(funcType, name, expFormula, min, max);

    // --- Set parameters ---
    for (int i = 0; i < params; ++i)
    {
        function->getFunction()->SetParName(i, paramNames[i].c_str());
        function->getFunction()->SetParameter(i, paramValues[i]);
        function->getFunction()->SetParError(i, paramErrors[i]);
    }

    std::getline(stream, line);
    if (line.find("Systematics:") != std::string::npos)
    {
        int nSys = 0;
        std::istringstream(line.substr(12)) >> nSys;
        //std::cout << "nSystematics: " << nSys << std::endl;
        for (int s = 0; s < nSys; ++s)
        {
            std::string sysName;
            std::getline(stream, line);
            if (!(line.find("  Systematic:") == 0))
            {
                continue;
            }
            sysName = line.substr(13); // Extract name after "  Systematic:"
            trim(sysName);

            std::vector<double> upParams;
            std::vector<double> downParams;

            // --- Up variation ---
            std::getline(stream, line);
            if (line.find("    UpParameters:") == 0)
            {
                std::istringstream ss(line.substr(17));
                double val;
                while (ss >> val)
                {
                    upParams.push_back(val);
                }
            }

            // --- Down variation ---
            std::getline(stream, line);
            if (line.find("    DownParameters:") == 0)
            {
                std::istringstream ss(line.substr(19));
                double val;
                while (ss >> val)
                {
                    downParams.push_back(val);
                }
            }

            // --- Register these in the FitFunction ---
            if (!upParams.empty() || !downParams.empty())
            {
                function->addSystematic(sysName, upParams, downParams);
            }
        }
    }
    func = function;
    //std::cout << "Successfully read: " << name << " (" << params << " parameters)\n\n";

    return stream;
}

// Systematics Implementation

void SimpleFitFunction::addSystematic(const std::string &sysName, const TF1 &upFunction, const TF1 &downFunction)
{
    systematics[sysName] = std::make_pair(upFunction, downFunction);
}

void SimpleFitFunction::addSystematic(const std::string &sysName, const std::vector<double> &upParams,
                                const std::vector<double> &downParams)
{

    TF1 *upClone = (TF1 *)function.Clone((std::string(function.GetName()) + "_" + sysName + "_up").c_str());
    TF1 *downClone = (TF1 *)function.Clone((std::string(function.GetName()) + "_" + sysName + "_down").c_str());

    for (size_t i = 0; i < upParams.size(); ++i)
    {
        upClone->SetParameter(i, upParams[i]);
    }
    for (size_t i = 0; i < downParams.size(); ++i)
    {
        downClone->SetParameter(i, downParams[i]);
    }

    systematics[sysName] = std::make_pair(*upClone, *downClone);
}

const TF1 *SimpleFitFunction::getSystematic(const std::string &sysName, bool up) const
{
    auto it = systematics.find(sysName);
    if (it == systematics.end())
    {
        return nullptr;
    }
    return up ? &(it->second.first) : &(it->second.second);
}

std::vector<std::string> SimpleFitFunction::listSystematics() const
{
    std::vector<std::string> names;
    names.reserve(systematics.size());
    for (const auto &kv : systematics)
    {
        names.push_back(kv.first);
    }
    return names;
}
