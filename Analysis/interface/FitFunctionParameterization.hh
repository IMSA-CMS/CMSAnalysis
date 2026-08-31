#ifndef FIT_FUNCTION_PARAMETERIZATION_HH
#define FIT_FUNCTION_PARAMETERIZATION_HH

#include "FitFunction.hh"
#include <optional>
#include <string>
#include <vector>

class FitFunctionParameterization : public FitFunctionBase
{
  public:
    FitFunctionParameterization() = default;
    FitFunctionParameterization(std::string name, std::string channelName, FunctionType functionType,
                                std::string expFormula, double min, double max);

    static FitFunctionParameterization load(const std::string &fileName);

    void insert(const FitFunction &function);
    FitFunction reconstructFunction(double mass);
    std::string getNormExpression(const std::string &variable) override;
    void save(const std::string &fileName, bool append = false);

  private:
    static std::optional<size_t> defaultNormParameterIndex(FunctionType type);

    std::string channelName;
    std::string expFormula;
    double min = 0;
    double max = 0;
    std::vector<FitFunction> parameterFunctions;
    std::optional<size_t> normParameterIndex;
};

#endif
