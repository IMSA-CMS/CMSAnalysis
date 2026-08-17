#ifndef FIT_FUNCTION_BASE_HH
#define FIT_FUNCTION_BASE_HH

#include <string>
#include <utility>

class FitFunctionBase
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

    virtual ~FitFunctionBase() = default;

    virtual std::string getName() = 0;

    FunctionType getFunctionType()
    {
        return functionType;
    }

    std::string getChannelName()
    {
        return channelName;
    }

  protected:
    FitFunctionBase() = default;
    FitFunctionBase(FunctionType functionType, std::string channelName)
        : channelName(std::move(channelName)), functionType(functionType)
    {
    }

    void setFunctionType(FunctionType type)
    {
        functionType = type;
    }

  private:
    std::string channelName;
    FunctionType functionType;
};

#endif
