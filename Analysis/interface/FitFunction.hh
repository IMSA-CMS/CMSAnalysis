#ifndef FIT_FUNCTION_HH
#define FIT_FUNCTION_HH

#include <map>
#include <string>
#include <vector>

class FitFunction
{
  public:
    enum class FunctionType
    {
        ExpressionFormula,
        DoubleSidedCrystalBall,
        PowerLaw,
        DoubleGaussian,
        GausLogPowerNorm,
        Voigt,
    };

    using NuisanceValues = std::map<std::string, double>;

    virtual ~FitFunction() {};

    std::string getName() const
    {
        return name;
    }

    FunctionType getFunctionType() const
    {
        return functionType;
    }

    virtual double evaluate(double observable, double modelMass,
                            const NuisanceValues &nuisances = {}) const = 0;
    virtual std::string getNormExpression(const std::string &variable) const = 0;
    virtual std::vector<std::string> listSystematics() const = 0;

    static std::string encodeName(std::map<std::string, std::string> parameters);
    static std::map<std::string, std::string> decodeName(std::string name);

  protected:
    FitFunction() {};
    FitFunction(FunctionType functionType, std::string name);

    void setName(std::string newName);
    void setFunctionType(FunctionType type)
    {
        functionType = type;
    }

  private:
    FunctionType functionType = FunctionType::ExpressionFormula;
    std::string name;
};

#endif
