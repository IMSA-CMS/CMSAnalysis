#include "../interface/FitFunction.hh"
#include "TF1.h"
#include <TMath.h>
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

double FitFunction::powerLaw(double *x, double *par)
{
    return par[0] * pow(x[0] - par[1], par[2]);
}

double FitFunction::DSCB(double *x, double *par)
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

    double root2 = std::pow(2, 0.5);
    if (-alpha_l <= t && alpha_h >= t)
    {
        result = exp(-0.5 * t * t);
    }
    else if (t < -alpha_l)
    {
        result = exp(-0.5 * alpha_l * alpha_l) * pow(fact1TLessMinosAlphaL * fact2TLessMinosAlphaL, -n_l);
    }
    else // if (t > alpha_h)
    {
        result = exp(-0.5 * alpha_h * alpha_h) * pow(fact1THihgerAlphaH * fact2THigherAlphaH, -n_h);
    }

    const double lowTailNorm = (n_l / std::abs(alpha_l)) * 1 / (n_l - 1) * std::exp(-0.5 * alpha_l * alpha_l);
    const double highTailNorm = (n_h / std::abs(alpha_h)) * 1 / (n_h - 1) * std::exp(-0.5 * alpha_h * alpha_h);
    const double gaussianNormA = erf(std::abs(alpha_l / root2)) + erf(std::abs(alpha_h / root2));
    const double gaussianNormB = std::pow(M_PI / 2, 0.5) * gaussianNormA;
    const double functionNormalization = std::pow(sigma * (gaussianNormB + lowTailNorm + highTailNorm), -1);

    // globalCounter++;
    // if(globalCounter%1000 == 0){
    // //std::cout<<"Global norm: " << globalNorm << "\n";
    // }
    return N * functionNormalization * result;
}

double FitFunction::doubleGaussian(double *x, double *par)
{
    return par[0] * TMath::Gaus(x[0], par[1], par[2]) + par[3] * TMath::Gaus(x[0], par[4], par[5]);
}

// Params: N, u, sigma1, s, n
double FitFunction::gausLogPowerNorm(double *xs, double *par)
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
    else
    {
        return mult * exp(-s * pow(log(x / u), n));
    }
}

// Params: N, mu, width (Breit-Wigner), sigma (Gaussian)
double FitFunction::voigt(double *x, double *par)
{
    return par[0] * TMath::Voigt(x[0] - par[1], par[3], par[2]);
}

FitFunction::FitFunction(const TF1 &func, FunctionType funcType)
    : function(func), functionType(funcType)
{
}

TF1 *FitFunction::getFunction()
{
    return &function;
}

const TF1 *FitFunction::getFunction() const
{
    return &function;
}

void FitFunction::setFunction(const TF1 &func, FunctionType funcType)
{
    function = func;
    functionType = funcType;
}

FitFunction::FunctionType FitFunction::getFunctionType() const
{
    return functionType;
}

std::string FitFunction::getName() const
{
    return function.GetName();
}

double FitFunction::getMin() const
{
    double min;
    double max;
    function.GetRange(min, max);
    return min;
}

double FitFunction::getMax() const
{
    double min;
    double max;
    function.GetRange(min, max);
    return max;
}

FitFunction FitFunction::createFunctionOfType(FunctionType functionType, const std::string &name,
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

    return FitFunction(func, functionType);
}

double FitFunction::evaluate(double x) const
{
    auto* tf1 = getFunction();
    double result = tf1->Eval(x);
    return result;
}

// Helper function for splitting strings
std::vector<std::string> FitFunction::split(const std::string &str, char delimiter)
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
std::ostream &operator<<(std::ostream &stream, FitFunction &function)
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

std::istream &operator>>(std::istream &stream, FitFunction &func)
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
    FitFunction function = FitFunction::createFunctionOfType(funcType, name, expFormula, min, max);

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
        std::cout << "nSystematics: " << nSys << std::endl;
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
    std::cout << "Successfully read: " << name << " (" << params << " parameters)\n\n";

    return stream;
}

// Systematics Implementation

void FitFunction::addSystematic(const std::string &sysName, const TF1 &upFunction, const TF1 &downFunction)
{
    systematics[sysName] = std::make_pair(upFunction, downFunction);
}

void FitFunction::addSystematic(const std::string &sysName, const std::vector<double> &upParams,
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

const TF1 *FitFunction::getSystematic(const std::string &sysName, bool up) const
{
    auto it = systematics.find(sysName);
    if (it == systematics.end())
    {
        return nullptr;
    }
    return up ? &(it->second.first) : &(it->second.second);
}

std::vector<std::string> FitFunction::listSystematics() const
{
    std::vector<std::string> names;
    names.reserve(systematics.size());
    for (const auto &kv : systematics)
    {
        names.push_back(kv.first);
    }
    return names;
}

std::string FitFunction::encodeName(std::map<std::string, std::string> parameters)
{
    std::string result;
    for (auto& [key, value] : parameters)
    {
        result += key + " - " + value + " | ";
    }
    return result;
}

std::map<std::string, std::string> FitFunction::decodeName(std::string name)
{
    std::map<std::string, std::string> result;
    std::istringstream stream(name);
    std::string token;
    while (std::getline(stream, token, '|'))
    {
        size_t dashPos = token.find(" - ");
        if (dashPos != std::string::npos)
        {
            std::string key = token.substr(0, dashPos);
            std::string value = token.substr(dashPos + 3);
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            result[key] = value;
        }
    }
    return result;
}