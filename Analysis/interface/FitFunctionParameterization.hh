#ifndef FIT_FUNCTION_PARAMETERIZATION_HH
#define FIT_FUNCTION_PARAMETERIZATION_HH

#include "FitFunction.hh"
#include "RooFitFunction.hh"
#include <optional>
#include <string>
#include <vector>

class RooAbsReal;

class FitFunctionParameterization
{
  public:
    FitFunctionParameterization() = default;
    FitFunctionParameterization(std::string name, std::string channelName, FitFunction::FunctionType functionType,
                                std::string expFormula, double min, double max);

    static FitFunctionParameterization load(const std::string &fileName);

    void insert(const FitFunction &function);
    FitFunction reconstructFunction(double mass);
    RooFitFunction reconstructFunction(RooAbsReal &observable, RooAbsReal &mass);
    void save(const std::string &fileName, bool append = false);

  private:
    static std::optional<size_t> defaultNormParameterIndex(FitFunction::FunctionType type);

    std::string name;
    std::string channelName;
    FitFunction::FunctionType functionType = FitFunction::FunctionType::ExpressionFormula;
    std::string expFormula;
    double min = 0;
    double max = 0;
    std::vector<FitFunction> parameterFunctions;
    std::optional<size_t> normParameterIndex;
};

#endif
