#include "CMSAnalysis/Analysis/interface/RooFitFunction.hh"
#include <RooAbsPdf.h>
#include <RooRealProxy.h>
#include <stdexcept>
#include <utility>

namespace
{
void setParameters(FitFunction &function, const std::vector<FitFunction> &parameterFunctions, const double mass)
{
    auto *const tf1 = function.getFunction();

    for (size_t i = 0; i < parameterFunctions.size(); ++i)
    {
        tf1->SetParameter(static_cast<int>(i), parameterFunctions[i].evaluate(mass));
    }
}

class RooParameterizedFunction final : public RooAbsPdf
{
  public:
    RooParameterizedFunction(const char *name, FitFunction::FunctionType functionType, std::string expFormula,
                             const double min, const double max, std::string channelName,
                             std::vector<FitFunction> parameterFunctions, RooAbsReal &observable, RooAbsReal &mass)
        : RooAbsPdf(name, name),
          observable("observable", "observable", this, observable),
          mass("mass", "mass", this, mass),
          function(FitFunction::createFunctionOfType(functionType, name, expFormula, min, max,
                                                     std::move(channelName))),
          parameterFunctions(std::move(parameterFunctions))
    {
    }

    RooParameterizedFunction(const RooParameterizedFunction &other, const char *name = nullptr)
        : RooAbsPdf(other, name),
          observable("observable", this, other.observable),
          mass("mass", this, other.mass),
          function(other.function),
          parameterFunctions(other.parameterFunctions)
    {
    }

    TObject *clone(const char *newName) const override
    {
        return new RooParameterizedFunction(*this, newName);
    }

  protected:
    double evaluate() const override
    {
        setParameters(function, parameterFunctions, static_cast<double>(mass));
        return function.evaluate(static_cast<double>(observable));
    }

  private:
    RooRealProxy observable;
    RooRealProxy mass;
    FitFunction function;
    std::vector<FitFunction> parameterFunctions;
};

class RooParameterizedNorm final : public RooAbsReal
{
  public:
    RooParameterizedNorm(const char *name, FitFunction::FunctionType functionType, std::string expFormula,
                         const double min, const double max, std::string channelName,
                         std::vector<FitFunction> parameterFunctions, RooAbsReal &mass,
                         const std::optional<size_t> normParameterIndex)
        : RooAbsReal(name, name),
          mass("mass", "mass", this, mass),
          function(FitFunction::createFunctionOfType(functionType, name, expFormula, min, max,
                                                     std::move(channelName))),
          min(min),
          max(max),
          parameterFunctions(std::move(parameterFunctions)),
          normParameterIndex(normParameterIndex)
    {
        if (normParameterIndex && *normParameterIndex >= this->parameterFunctions.size())
        {
            throw std::out_of_range("Normalization parameter index is out of range");
        }
    }

    RooParameterizedNorm(const RooParameterizedNorm &other, const char *name = nullptr)
        : RooAbsReal(other, name),
          mass("mass", this, other.mass),
          function(other.function),
          min(other.min),
          max(other.max),
          parameterFunctions(other.parameterFunctions),
          normParameterIndex(other.normParameterIndex)
    {
    }

    TObject *clone(const char *newName) const override
    {
        return new RooParameterizedNorm(*this, newName);
    }

  protected:
    double evaluate() const override
    {
        const auto massValue = static_cast<double>(mass);
        if (normParameterIndex)
        {
            return parameterFunctions[*normParameterIndex].evaluate(massValue);
        }

        setParameters(function, parameterFunctions, massValue);
        return function.getFunction()->Integral(min, max);
    }

  private:
    RooRealProxy mass;
    mutable FitFunction function;
    double min;
    double max;
    std::vector<FitFunction> parameterFunctions;
    std::optional<size_t> normParameterIndex;
};
} // namespace

RooFitFunction::RooFitFunction(std::string name, std::string channelName,
                               const FitFunction::FunctionType functionType,
                               std::unique_ptr<RooAbsReal> function, std::unique_ptr<RooAbsReal> norm)
    : FitFunctionBase(functionType, std::move(channelName)),
      name(std::move(name)),
      function(std::move(function)),
      norm(std::move(norm))
{
    if (!this->function || !this->norm)
    {
        throw std::invalid_argument("RooFitFunction requires both a function and a normalization");
    }
}

RooFitFunction RooFitFunction::createParameterized(const std::string &name, const std::string &channelName,
                                                   const FitFunction::FunctionType functionType,
                                                   const std::string &expFormula,
                                                   const double min, const double max,
                                                   const std::vector<FitFunction> &parameterFunctions,
                                                   RooAbsReal &observable, RooAbsReal &mass,
                                                   const std::optional<size_t> normParameterIndex)
{
    auto function = std::make_unique<RooParameterizedFunction>(name.c_str(), functionType, expFormula, min, max,
                                                               channelName, parameterFunctions, observable, mass);
    auto norm = std::make_unique<RooParameterizedNorm>((name + "_norm").c_str(), functionType, expFormula, min, max,
                                                       channelName, parameterFunctions, mass, normParameterIndex);
    return RooFitFunction(name, channelName, functionType, std::move(function), std::move(norm));
}

RooAbsReal &RooFitFunction::getFunction()
{
    return *function;
}

const RooAbsReal &RooFitFunction::getFunction() const
{
    return *function;
}

RooAbsReal &RooFitFunction::getNorm()
{
    return *norm;
}

const RooAbsReal &RooFitFunction::getNorm() const
{
    return *norm;
}

std::string RooFitFunction::getName()
{
    return name;
}
