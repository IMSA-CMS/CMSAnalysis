#include "../interface/SimpleFitFunction.hh"
#include "TF1.h"
#include "TBuffer.h"
#include <TMath.h>
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
        switch (getFunctionType())
        {
        case FunctionType::DoubleSidedCrystalBall: tf1.SetFunction(DSCB); break;
        case FunctionType::PowerLaw: tf1.SetFunction(powerLaw); break;
        case FunctionType::DoubleGaussian: tf1.SetFunction(doubleGaussian); break;
        case FunctionType::GausLogPowerNorm: tf1.SetFunction(gausLogPowerNorm); break;
        case FunctionType::Voigt: tf1.SetFunction(voigt); break;
        case FunctionType::ExpressionFormula: break;
        }
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
    function = func;
    setName(func.GetName());
    setFunctionType(funcType);
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


double SimpleFitFunction::powerLaw(double *x, double *par)
{
    return par[0] * pow(x[0] - par[1], par[2]);
}

double SimpleFitFunction::DSCB(double *x, double *par)
{
    const double alpha_l = par[0];
    const double alpha_h = par[1];
    const double n_l = par[2];
    const double n_h = par[3];
    const double mean = par[4];
    const double sigma = par[5];
    const double N = par[6];
    const float t = (x[0] - mean) / sigma;
    double result;
    const double fact1TLessMinosAlphaL = alpha_l / n_l;
    const double fact2TLessMinosAlphaL = (n_l / alpha_l) - alpha_l - t;
    const double fact1THihgerAlphaH = alpha_h / n_h;
    const double fact2THigherAlphaH = (n_h / alpha_h) - alpha_h + t;

    const double root2 = std::pow(2, 0.5);
    if (-alpha_l <= t && alpha_h >= t)
    {
        result = exp(-0.5 * t * t);
    }
    else if (t < -alpha_l)
    {
        result = exp(-0.5 * alpha_l * alpha_l) * pow(fact1TLessMinosAlphaL * fact2TLessMinosAlphaL, -n_l);
    }
    else
    {
        result = exp(-0.5 * alpha_h * alpha_h) * pow(fact1THihgerAlphaH * fact2THigherAlphaH, -n_h);
    }

    const double lowTailNorm = (n_l / std::abs(alpha_l)) / (n_l - 1) * std::exp(-0.5 * alpha_l * alpha_l);
    const double highTailNorm = (n_h / std::abs(alpha_h)) / (n_h - 1) * std::exp(-0.5 * alpha_h * alpha_h);
    const double gaussianNormA = erf(std::abs(alpha_l / root2)) + erf(std::abs(alpha_h / root2));
    const double gaussianNormB = std::pow(M_PI / 2, 0.5) * gaussianNormA;
    const double functionNormalization = std::pow(sigma * (gaussianNormB + lowTailNorm + highTailNorm), -1);
    return N * functionNormalization * result;
}

double SimpleFitFunction::doubleGaussian(double *x, double *par)
{
    return par[0] * TMath::Gaus(x[0], par[1], par[2]) + par[3] * TMath::Gaus(x[0], par[4], par[5]);
}

double SimpleFitFunction::gausLogPowerNorm(double *xs, double *par)
{
    const auto x = xs[0];
    const auto mult = par[0];
    const auto u = par[1];
    const auto sigma1 = par[2];
    const auto s = par[3];
    const auto n = par[4];

    if (x <= u)
    {
        return mult * exp(-(x - u) * (x - u) / (2 * sigma1 * sigma1));
    }
    return mult * exp(-s * pow(log(x / u), n));
}

double SimpleFitFunction::voigt(double *x, double *par)
{
    return par[0] * TMath::Voigt(x[0] - par[1], par[3], par[2]);
}

SimpleFitFunction SimpleFitFunction::createFunctionOfType(FunctionType functionType, const std::string &name,
                                              const std::string &expFormula, double min, double max)
{
    TF1 func;
    switch (functionType)
    {
    case FunctionType::ExpressionFormula:
        func = TF1(name.data(), expFormula.data(), min, max, TF1::EAddToList::kNo);
        break;
    case FunctionType::DoubleSidedCrystalBall:
        func = TF1(name.data(), DSCB, min, max, 7, 1, TF1::EAddToList::kNo);
        func.SetParNames("#alpha_{low}", "#alpha_{high}", "n_{low}", "n_{high}", "#mu", "#sigma", "norm");
        break;
    case FunctionType::PowerLaw:
        func = TF1(name.data(), powerLaw, min, max, 3, 1, TF1::EAddToList::kNo);
        break;
    case FunctionType::DoubleGaussian:
        func = TF1(name.data(), doubleGaussian, min, max, 6, 1, TF1::EAddToList::kNo);
        func.SetParNames("mul_{1}", "#mu_{1}", "#sigma_{1}", "mul_{2}", "#mu_{2}", "#sigma_{2}");
        break;
    case FunctionType::GausLogPowerNorm:
        func = TF1(name.data(), gausLogPowerNorm, min, max, 5, 1, TF1::EAddToList::kNo);
        func.SetParNames("N", "#mu", "#sigma_{1}", "s", "n");
        break;
    case FunctionType::Voigt:
        func = TF1(name.data(), voigt, min, max, 4, 1, TF1::EAddToList::kNo);
        func.SetParNames("N", "#mu", "#Gamma", "#sigma");
        break;
    };

    return SimpleFitFunction(func, functionType);
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
            if (getFunctionType() == FunctionType::DoubleSidedCrystalBall && p == 6)
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
    double norm = 0;
    switch (getFunctionType())
    {
    case FunctionType::DoubleSidedCrystalBall:
        norm = function.GetParameter(6);
        break;
    case FunctionType::Voigt:
        norm = function.GetParameter(0);
        break;
    case FunctionType::PowerLaw:
    {
        const double coeff = function.GetParameter(0);
        const double shift = function.GetParameter(1);
        const double expo = function.GetParameter(2);
        const double lower = getMin() - shift;
        const double upper = getMax() - shift;

        if (expo != -1)
        {
            norm = coeff / (expo + 1) * (std::pow(upper, expo + 1) - std::pow(lower, expo + 1));
        }
        else
        {
            norm = coeff * std::log(upper / lower);
            
        }
        break;
    }
    case FunctionType::GausLogPowerNorm:
    case FunctionType::DoubleGaussian:
    case FunctionType::ExpressionFormula:
    default:
        norm = function.Integral(getMin(), getMax());
        break;
    }

    std::ostringstream expression;
    expression << std::setprecision(17) << norm;
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
std::ostream &operator<<(std::ostream &stream, SimpleFitFunction &function)
{
    TF1 *func = function.getFunction();
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

std::istream &operator>>(std::istream &stream, SimpleFitFunction &func)
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
    SimpleFitFunction function = SimpleFitFunction::createFunctionOfType(funcType, name, expFormula, min, max);

    // --- Set parameters ---
    for (int i = 0; i < params; ++i)
    {
        function.getFunction()->SetParName(i, paramNames[i].c_str());
        function.getFunction()->SetParameter(i, paramValues[i]);
        function.getFunction()->SetParError(i, paramErrors[i]);
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
                function.addSystematic(sysName, upParams, downParams);
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

ClassImp(SimpleFitFunction)