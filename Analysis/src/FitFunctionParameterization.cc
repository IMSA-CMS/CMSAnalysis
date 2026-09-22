#include "../interface/FitFunctionParameterization.hh"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <set>
#include <stdexcept>
#include <utility>

FitFunctionParameterization::FitFunctionParameterization(std::string name, std::string channelName,
                                                         const FunctionType functionType,
                                                         std::string expFormula, const double min, const double max)
    : FitFunction(functionType, std::move(name)),
      channelName(std::move(channelName)),
      expFormula(std::move(expFormula)),
      min(min),
      max(max),
      templateFunction(SimpleFitFunction::createFunctionOfType(functionType, getName(), this->expFormula, min, max)),
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

    FitFunctionParameterization result(objectName, channel, static_cast<FunctionType>(type), formula,
                                       min, max);
    file >> label;
    if (label == "NormParameterIndex:")
    {
        int normIndex = -1;
        file >> normIndex;
        result.normParameterIndex = normIndex;
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

        auto function = SimpleFitFunction::createFunctionOfType(static_cast<FunctionType>(parameterType), functionName, parameterFormula, functionMin, functionMax);
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
        result.parameterFunctions.push_back(std::move(function));
    }

    if (!file && !file.eof())
    {
        throw std::runtime_error("Invalid FitFunctionParameterization in " + fileName);
    }
    return result;
}

void FitFunctionParameterization::insert(const SimpleFitFunction &function)
{
    parameterFunctions.push_back(function);
}

double FitFunctionParameterization::evaluate(const double observable, const double modelMass,
                                             const NuisanceValues &nuisances) const
{
    if (parameterFunctions.size() != static_cast<size_t>(templateFunction.getFunction()->GetNpar()))
    {
        throw std::runtime_error("FitFunctionParameterization has a different number of parameter functions than its model");
    }

    std::vector<double> parameters;
    parameters.reserve(parameterFunctions.size());
    for (size_t parameter = 0; parameter < parameterFunctions.size(); ++parameter)
    {
        const auto &parameterFunction = parameterFunctions[parameter];
        if (getFunctionType() == FunctionType::DoubleSidedCrystalBall && parameter == 6)
        {
            // so here the variations to the shape are applied to the pdf shape.
            // normalization shoul go through get norm expression and roofit yield formula
         
            parameters.push_back(parameterFunction.evaluate(modelMass));
        }
        else
        {
            const double nominal = parameterFunction.evaluate(modelMass);
            double value = nominal;
            for (const auto &[name, delta] : nuisances)
            {
                if (!std::isfinite(delta))
                    throw std::invalid_argument("Shape-systematic deltas must be finite");

                // eval each endpoint before interpolating shape parameter
                // do this rather than interpolating the coefficients of its mass fit
                // other way is backwards I think
                const double variation = parameterFunction.evaluate(modelMass, {{name, delta >= 0 ? 1.0 : -1.0}});
                value += std::abs(delta) * (variation - nominal);
            }
            parameters.push_back(value);
        }
    }
    return templateFunction.evaluateWithParameters(observable, parameters);
}

std::string FitFunctionParameterization::getNormExpression(const std::string &variable) const
{
    if (normParameterIndex < 0 || static_cast<size_t>(normParameterIndex) >= parameterFunctions.size())
    {
        throw std::runtime_error("FitFunction type does not have a norma parameter");
    }
    //i have to figure out how to get it for ones without norm parameer
    return parameterFunctions[normParameterIndex].getExpression(variable);
}

std::vector<std::string> FitFunctionParameterization::listSystematics() const
{
    std::set<std::string> names;
    for (const auto &parameterFunction : parameterFunctions)
    {
        for (const auto &name : parameterFunction.listSystematics())
        {
            names.insert(name);
        }
    }
    return {names.begin(), names.end()};
}

void FitFunctionParameterization::save(const std::string &fileName, const bool append)
{
    std::ofstream file(fileName, append ? std::ios::app : std::ios::out);
    if (!file)
    {
        throw std::invalid_argument("File " + fileName + " could not be opened");
    }

    file << "Parameterization: " << std::quoted(getName()) << '\n';
    file << "Channel: " << std::quoted(channelName) << '\n';
    file << "OriginalFunctionTypeEnum: " << static_cast<int>(getFunctionType()) << '\n';
    file << "OriginalExpressionFormula: " << std::quoted(expFormula) << '\n';
    file << "OriginalRange: " << min << ' ' << max << '\n';
    file << "NormParameterIndex: "
         << normParameterIndex << '\n';
    file << "NumOfParameters: " << parameterFunctions.size() << '\n';

    for (auto &parameterFunction : parameterFunctions)
    {
        auto *const tf1 = parameterFunction.getFunction();
        double functionMin = 0;
        double functionMax = 0;
        tf1->GetRange(functionMin, functionMax);
        const char *const rawFormula = tf1->GetExpFormula();
        const std::string formula = rawFormula == nullptr ? "" : rawFormula;

        file << "Parameter: " << std::quoted(parameterFunction.getParameter("Parameter")) << '\n';
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

int
FitFunctionParameterization::defaultNormParameterIndex(const FunctionType type)
{
    switch (type)
    {
    case FunctionType::DoubleSidedCrystalBall:
        return 6;
    case FunctionType::GausLogPowerNorm:
    case FunctionType::Voigt:
        return 0;
    case FunctionType::PowerLaw:
    case FunctionType::ExpressionFormula:
    case FunctionType::DoubleGaussian:
    default:
        return -1;
    }
}

ClassImp(FitFunctionParameterization)