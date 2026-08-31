#ifndef FIT_FUNCTION_HH
#define FIT_FUNCTION_HH

#include "CMSAnalysis/Analysis/interface/FitFunctionBase.hh"
#include "TF1.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class FitFunction : public FitFunctionBase
{
  public:
    // static const std::vector<std::string> functionList;
    static FitFunction createFunctionOfType(FunctionType functionType, const std::string &name,
                                            const std::string &expFormula, double min, double max);
    // static std::vector<FitFunction> loadFunctions(const std::string& fileName);
    // static void saveFunctions(std::vector<FitFunction>& functions, const std::string& fileName);
    FitFunction() {};
    FitFunction(const TF1 &func, FunctionType funcType);
    TF1 *getFunction();
    void setFunction(const TF1 &function, FunctionType funcType);
    double getMin();
    double getMax();

    std::string getParameter(std::string name);
    std::string getParameterName();

    double evaluate(double x);
    std::string getExpression(const std::string &variable);
    std::string getNormExpression(const std::string &variable) override;

    void addSystematic(const std::string &sysName, const TF1 &upFunction, const TF1 &downFunction);
    void addSystematic(const std::string &sysName, const std::vector<double> &upParams,
                       const std::vector<double> &downParams);
    const TF1 *getSystematic(const std::string &sysName, bool up) const;
    std::vector<std::string> listSystematics() const;

  private:
    TF1 function;
    std::map<std::string, std::pair<TF1, TF1>> systematics;

    static double powerLaw(double *x, double *par);
    static double DSCB(double *x, double *par);
    static double doubleGaussian(double *x, double *par);
    static double gausLogPowerNorm(double *x, double *par);
    static double voigt(double *x, double *par);
    static std::vector<std::string> split(const std::string &str, char delimiter);
};

std::ostream &operator<<(std::ostream &stream, FitFunction &function);
std::istream &operator>>(std::istream &stream, FitFunction &function);

#endif
