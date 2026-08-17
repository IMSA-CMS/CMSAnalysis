#include "CMSAnalysis/Analysis/interface/FitFunctionParameterization.hh"
#include <RooAbsReal.h>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <utility>

FitFunctionParameterization::FitFunctionParameterization(std::string name, std::string channelName,
                                                         const FitFunction::FunctionType functionType,
                                                         std::string expFormula, const double min, const double max)
    : name(std::move(name)),
      channelName(std::move(channelName)),
      functionType(functionType),
      expFormula(std::move(expFormula)),
      min(min),
      max(max),
      normParameterIndex(defaultNormParameterIndex(functionType))
{
}

FitFunctionParameterization FitFunctionParameterization::load(const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file)
    {
        throw std::runtime_error("File not loaded successfully: " + fileName);
    }

    std::string label;
    std::string objectName;
    std::string channel;
    std::string formula;
    int type = 0;
    double min = 0;
    double max = 0;
    size_t size = 0;
    file >> label >> std::quoted(objectName);
    file >> label >> std::quoted(channel);
    file >> label >> type;
    file >> label >> std::quoted(formula);
    file >> label >> min >> max;

    FitFunctionParameterization result(objectName, channel, static_cast<FitFunction::FunctionType>(type), formula,
                                       min, max);
    file >> label;
    if (label == "NormParameterIndex:")
    {
        int normIndex = -1;
        file >> normIndex;
        result.normParameterIndex = normIndex < 0 ? std::nullopt : std::optional<size_t>(normIndex);
        file >> label;
    }
    file >> size;

    for (size_t i = 0; i < size; ++i)
    {
        std::string parameterName;
        std::string parameterFormula;
        std::string functionName;
        int parameterType = 0;
        double functionMin = 0;
        double functionMax = 0;
        int npar = 0;
        file >> label >> std::quoted(parameterName);
        file >> label >> parameterType;
        file >> label >> std::quoted(parameterFormula);
        file >> label >> std::quoted(functionName);
        file >> label >> functionMin >> functionMax;
        file >> label >> npar;

        auto function = FitFunction::createFunctionOfType(static_cast<FitFunction::FunctionType>(parameterType),
                                                          functionName, parameterFormula, functionMin, functionMax,
                                                          channel);
        for (int parameter = 0; parameter < npar; ++parameter)
        {
            std::string parameterName;
            double value = 0;
            double error = 0;
            file >> label >> std::quoted(parameterName) >> value >> error;
            function.getFunction()->SetParName(parameter, parameterName.c_str());
            function.getFunction()->SetParameter(parameter, value);
            function.getFunction()->SetParError(parameter, error);
        }
        if (function.getParameterName() != parameterName)
        {
            function.getFunction()->SetName((channel + "/" + parameterName).c_str());
        }
        result.parameterFunctions.push_back(std::move(function));
    }

    if (!file && !file.eof())
    {
        throw std::runtime_error("Invalid FitFunctionParameterization in " + fileName);
    }
    return result;
}

void FitFunctionParameterization::insert(const FitFunction &function)
{
    parameterFunctions.push_back(function);
}

FitFunction FitFunctionParameterization::reconstructFunction(const double mass)
{
    auto function = FitFunction::createFunctionOfType(functionType, name, expFormula, min, max, channelName);
    auto *const tf1 = function.getFunction();
    if (parameterFunctions.size() != static_cast<size_t>(tf1->GetNpar()))
    {
        throw std::runtime_error("FitFunction parameterization does not match the function parameter count");
    }

    for (size_t i = 0; i < parameterFunctions.size(); ++i)
    {
        tf1->SetParameter(static_cast<int>(i), parameterFunctions[i].evaluate(mass));
        tf1->SetParName(static_cast<int>(i), parameterFunctions[i].getParameterName().c_str());
    }
    return function;
}

RooFitFunction FitFunctionParameterization::reconstructFunction(RooAbsReal &observable, RooAbsReal &mass)
{
    return RooFitFunction::createParameterized(name, channelName, functionType, expFormula, min, max,
                                               parameterFunctions, observable, mass, normParameterIndex);
}

void FitFunctionParameterization::save(const std::string &fileName, const bool append)
{
    std::ofstream file(fileName, append ? std::ios::app : std::ios::out);
    if (!file)
    {
        throw std::invalid_argument("File " + fileName + " could not be opened");
    }

    file << "Parameterization: " << std::quoted(name) << '\n';
    file << "Channel: " << std::quoted(channelName) << '\n';
    file << "OriginalFunctionTypeEnum: " << static_cast<int>(functionType) << '\n';
    file << "OriginalExpressionFormula: " << std::quoted(expFormula) << '\n';
    file << "OriginalRange: " << min << ' ' << max << '\n';
    file << "NormParameterIndex: "
         << (normParameterIndex ? static_cast<int>(*normParameterIndex) : -1) << '\n';
    file << "NumOfParameters: " << parameterFunctions.size() << '\n';

    for (auto &parameterFunction : parameterFunctions)
    {
        auto *const tf1 = parameterFunction.getFunction();
        double functionMin = 0;
        double functionMax = 0;
        tf1->GetRange(functionMin, functionMax);
        const char *const rawFormula = tf1->GetExpFormula();
        const std::string formula = rawFormula == nullptr ? "" : rawFormula;

        file << "Parameter: " << std::quoted(parameterFunction.getParameterName()) << '\n';
        file << "FunctionTypeEnum: " << static_cast<int>(parameterFunction.getFunctionType()) << '\n';
        file << "ExpressionFormula: " << std::quoted(formula) << '\n';
        file << "FunctionName: " << std::quoted(std::string(tf1->GetName())) << '\n';
        file << "Range: " << functionMin << ' ' << functionMax << '\n';
        file << "NumOfTF1Parameters: " << tf1->GetNpar() << '\n';
        for (int parameter = 0; parameter < tf1->GetNpar(); ++parameter)
        {
            file << "TF1Parameter: " << std::quoted(std::string(tf1->GetParName(parameter))) << ' '
                 << tf1->GetParameter(parameter) << ' ' << tf1->GetParError(parameter) << '\n';
        }
    }
}

std::optional<size_t>
FitFunctionParameterization::defaultNormParameterIndex(const FitFunction::FunctionType type)
{
    switch (type)
    {
    case FitFunction::FunctionType::DoubleSidedCrystalBall:
        return 6;
    case FitFunction::FunctionType::GausLogPowerNorm:
    case FitFunction::FunctionType::Voigt:
        return 0;
    case FitFunction::FunctionType::ExpressionFormula:
    case FitFunction::FunctionType::PowerLaw:
    case FitFunction::FunctionType::DoubleGaussian:
        return std::nullopt;
    }
    return std::nullopt;
}
