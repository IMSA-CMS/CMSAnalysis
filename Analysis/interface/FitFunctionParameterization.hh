#ifndef FIT_FUNCTION_PARAMETERIZATION_HH
#define FIT_FUNCTION_PARAMETERIZATION_HH

#include "FitFunction.hh"
#include "SimpleFitFunction.hh"
#include <string>
#include <vector>

class FitFunctionParameterization : public FitFunction
{
  public:
    FitFunctionParameterization() = default;
    FitFunctionParameterization(std::string name, std::string channelName, FunctionType functionType,
                                std::string expFormula, double min, double max);

    static FitFunctionParameterization load(const std::string &fileName);
    static std::vector<FitFunctionParameterization> loadFunctions(const std::string &fileName);

    void insert(const SimpleFitFunction &function);
    //update evaluate
    double evaluate(double observable, double modelMass,
                    const NuisanceValues &nuisances = {}) const override;
    std::string getNormExpression(const std::string &variable) const override;
    std::vector<std::string> listSystematics() const override;
    void save(const std::string &fileName, bool append = false);

  private:
    static int defaultNormParameterIndex(FunctionType type);

    std::string channelName;
    std::string expFormula;
    double min = 0;
    double max = 0;
    SimpleFitFunction templateFunction;
    std::vector<SimpleFitFunction> parameterFunctions;
    // -1 means no said yield parameter
    int normParameterIndex = -1;
    ClassDef(FitFunctionParameterization, 1)
};

#endif
