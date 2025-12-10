#ifndef FIT_FUNCTION_HH
#define FIT_FUNCTION_HH

#include "TF1.h"
#include <iostream>

class FitFunction
{
  public:
    enum class FunctionType
    {
        ExpressionFormula,
        DoubleSidedCrystalBall,
        PowerLaw,
        DoubleGaussian,
    };

    // static const std::vector<std::string> functionList;
    static FitFunction createFunctionOfType(FunctionType functionType, const std::string &name,
                                            const std::string &expFormula, double min, double max,
                                            std::string channelName);

    // static std::vector<FitFunction> loadFunctions(const std::string& fileName);
    // static void saveFunctions(std::vector<FitFunction>& functions, const std::string& fileName);

    // static std::string getFormulaName(const std::string& name);

    FitFunction() {} // Needed to implement unordered_map - never used seriously
    FitFunction(const TF1 &func, FunctionType funcType, std::string channelName);
    TF1 *getFunction();
    void setFunction(const TF1 &func, FunctionType funcType);
    FunctionType getFunctionType();
    std::string getName();
    double getMin();
    double getMax();
    std::string getChannelName(); // Don't use
    std::string getChannel();
    std::string getParameterName();

  private:
    static double powerLaw(double *x, double *par);
    static double DSCB(double *x, double *par);
    static double doubleGaussian(double *x, double *par);
    static std::vector<std::string> split(const std::string &str, char delimiter);

    TF1 function;
    std::string channelName;
    FunctionType functionType;
};

std::ostream &operator<<(std::ostream &stream, FitFunction &function);
std::istream &operator>>(std::istream &stream, FitFunction &function);

#endif