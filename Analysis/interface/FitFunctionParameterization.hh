#ifndef FIT_FUNCTION_PARAMETERIZATION_HH
#define FIT_FUNCTION_PARAMETERIZATION_HH

#include "FitFunction.hh"
#include "SimpleFitFunction.hh"
#include <memory>
#include <mutex>
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

    void insert(std::shared_ptr<SimpleFitFunction> function);
    // Call after modifying an inserted parameter function directly.
    void clearCache() const;
    //update evaluate
    double evaluate(double observable, double modelMass,
                    const NuisanceValues &nuisances = {}) const override;
    bool hasAnalyticalIntegral() const override { return templateFunction->hasAnalyticalIntegral(); }
    double integral(double low, double high, double modelMass,
                    const NuisanceValues &nuisances = {}) const override;
    std::string getNormExpression(const std::string &variable) const override;
    std::vector<std::string> listSystematics() const override;
    void save(const std::string &fileName, bool append = false);

  private:
    // Copies start empty; PDF clones can share a model, so access is locked.
    struct ParameterCache
    {
        double mass = 0;
        NuisanceValues nuisances;
        std::vector<double> parameters;
        std::mutex mutex;

        ParameterCache() = default;
        ParameterCache(const ParameterCache &) {}
        ParameterCache &operator=(const ParameterCache &)
        {
            parameters.clear();
            return *this;
        }
    };

    mutable ParameterCache parameterCache; //!
    // Callers hold the cache lock while using these parameters.
    const std::vector<double> &parameterValues(double modelMass, const NuisanceValues &nuisances) const;
    std::string channelName;
    std::string expFormula;
    double min = 0;
    double max = 0;
    std::shared_ptr<SimpleFitFunction> templateFunction; //!
    std::vector<std::shared_ptr<SimpleFitFunction>> parameterFunctions; //!
    // -1 means no said yield parameter
    int normParameterIndex = -1;
    ClassDefOverride(FitFunctionParameterization, 2)
};

#endif
