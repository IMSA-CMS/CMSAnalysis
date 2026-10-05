#include "../interface/FitFunctionParameterization.hh"
#include <TBuffer.h>
#include <TClass.h>
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
      normParameterIndex(templateFunction->getNormParameterIndex())
{
}

FitFunctionParameterization FitFunctionParameterization::load(const std::string &fileName)
{
    auto functions = loadFunctions(fileName);
    if (functions.empty())
    {
        throw std::runtime_error("No FitFunctionParameterization in " + fileName);
    }
    return functions[0];
}

std::vector<FitFunctionParameterization> FitFunctionParameterization::loadFunctions(const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file)
    {
        throw std::runtime_error("File not loaded successfully: " + fileName);
    }

    std::vector<FitFunctionParameterization> functions;
    std::string label;
    while (file >> label)
    {
        if (label != "Parameterization:")
        {
            throw std::runtime_error("Invalid FitFunctionParameterization in " + fileName);
        }

        std::string objectName;
        std::string channel;
        std::string formula;
        int type = 0;
        double min = 0;
        double max = 0;
        size_t size = 0;
        file >> std::quoted(objectName);
        file >> label >> std::quoted(channel);
        file >> label >> type;
        file >> label >> std::quoted(formula);
        file >> label >> min >> max;

        FitFunctionParameterization result(objectName, channel, static_cast<FunctionType>(type), formula,
                                           min, max);
        file >> label;
        if (label == "NormParameterIndex:")
        {
            file >> result.normParameterIndex;
            file >> label;
        }
        file >> size;
        if (!file)
        {
            throw std::runtime_error("Invalid FitFunctionParameterization in " + fileName);
        }

        for (size_t i = 0; i < size; ++i)
        {
            std::shared_ptr<SimpleFitFunction> function;
            file >> function;
            if (!file)
            {
                throw std::runtime_error("Invalid parameter function in " + fileName);
            }
            result.parameterFunctions.push_back(std::move(function));
        }
        functions.push_back(std::move(result));
    }

    if (!file && !file.eof())
    {
        throw std::runtime_error("Invalid FitFunctionParameterization in " + fileName);
    }
    return functions;
}

void FitFunctionParameterization::insert(std::shared_ptr<SimpleFitFunction> function)
{
    parameterFunctions.push_back(std::move(function));
}

double FitFunctionParameterization::evaluate(const double observable, const double modelMass,
                                             const NuisanceValues &nuisances) const
{
    if (parameterFunctions.size() != static_cast<size_t>(templateFunction->getFunction()->GetNpar()))
    {
        throw std::runtime_error("FitFunctionParameterization has a different number of parameter functions than its model");
    }

    std::vector<double> parameters;
    parameters.reserve(parameterFunctions.size());
    for (size_t parameter = 0; parameter < parameterFunctions.size(); ++parameter)
    {
        const auto &parameterFunction = parameterFunctions[parameter];
        if (!templateFunction->variesWithSystematic(parameter))
        {
            // so here the variations to the shape are applied to the pdf shape.
            // normalization shoul go through get norm expression and roofit yield formula
         
            parameters.push_back(parameterFunction->evaluate(modelMass));
        }
        else
        {
            const double nominal = parameterFunction->evaluate(modelMass);
            double value = nominal;
            for (const auto &[name, delta] : nuisances)
            {
                if (!std::isfinite(delta))
                    throw std::invalid_argument("Shape-systematic deltas must be finite");

                // eval each endpoint before interpolating shape parameter
                // do this rather than interpolating the coefficients of its mass fit
                // other way is backwards I think
                const double variation = parameterFunction->evaluate(modelMass, {{name, delta >= 0 ? 1.0 : -1.0}});
                value += std::abs(delta) * (variation - nominal);
            }
            parameters.push_back(value);
        }
    }
    return templateFunction->evaluateWithParameters(observable, parameters);
}

std::string FitFunctionParameterization::getNormExpression(const std::string &variable) const
{
    if (normParameterIndex < 0 || static_cast<size_t>(normParameterIndex) >= parameterFunctions.size())
    {
        throw std::runtime_error("FitFunction type does not have a norma parameter");
    }
    //i have to figure out how to get it for ones without norm parameer
    return parameterFunctions[normParameterIndex]->getExpression(variable);
}

std::vector<std::string> FitFunctionParameterization::listSystematics() const
{
    std::set<std::string> names;
    for (const auto &parameterFunction : parameterFunctions)
    {
        for (const auto &name : parameterFunction->listSystematics())
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
        file << *parameterFunction;
    }
}

void FitFunctionParameterization::Streamer(TBuffer &buffer)
{
    if (buffer.IsReading())
    {
        buffer.ReadClassBuffer(FitFunctionParameterization::Class(), this);
        auto *type = SimpleFitFunction::Class();
        templateFunction.reset(static_cast<SimpleFitFunction *>(buffer.ReadObjectAny(type)));
        unsigned int size = 0;
        buffer >> size;
        parameterFunctions.clear();
        parameterFunctions.reserve(size);
        for (unsigned int i = 0; i < size; ++i)
            parameterFunctions.emplace_back(static_cast<SimpleFitFunction *>(buffer.ReadObjectAny(type)));
    }
    else
    {
        buffer.WriteClassBuffer(FitFunctionParameterization::Class(), this);
        auto *type = SimpleFitFunction::Class();
        buffer.WriteObjectAny(templateFunction.get(), type);
        buffer << static_cast<unsigned int>(parameterFunctions.size());
        for (const auto &function : parameterFunctions)
            buffer.WriteObjectAny(function.get(), type);
    }
}

ClassImp(FitFunctionParameterization)
