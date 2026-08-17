#ifndef ROO_FIT_FUNCTION_HH
#define ROO_FIT_FUNCTION_HH

#include "FitFunction.hh"
#include "FitFunctionBase.hh"
#include <RooAbsReal.h>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class RooFitFunction : public FitFunctionBase
{
  public:
    RooFitFunction(std::string name, std::string channelName, FitFunction::FunctionType functionType,
                   std::unique_ptr<RooAbsReal> function, std::unique_ptr<RooAbsReal> norm);

    static RooFitFunction createParameterized(const std::string &name, const std::string &channelName,
                                              FitFunction::FunctionType functionType, const std::string &expFormula, double min,
                                              double max, const std::vector<FitFunction> &parameterFunctions,
                                              RooAbsReal &observable, RooAbsReal &mass,
                                              std::optional<size_t> normParameterIndex = std::nullopt);

    RooAbsReal &getFunction();
    const RooAbsReal &getFunction() const;
    RooAbsReal &getNorm();
    const RooAbsReal &getNorm() const;
    std::string getName() override;

  private:
    std::string name;
    std::unique_ptr<RooAbsReal> function;
    std::unique_ptr<RooAbsReal> norm;
};

#endif
