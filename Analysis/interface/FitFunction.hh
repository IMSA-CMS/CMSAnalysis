#ifndef FIT_FUNCTION_HH
#define FIT_FUNCTION_HH

#include <map>
#include <string>

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

    virtual ~FitFunction() {};

    std::string getName()
    {
        return name;
    }

    FunctionType getFunctionType()
    {
        return functionType;
    }

    virtual std::string getNormExpression(const std::string &variable) = 0;

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
