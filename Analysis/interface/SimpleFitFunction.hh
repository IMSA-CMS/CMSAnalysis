#ifndef SIMPLE_FIT_FUNCTION_HH
#define SIMPLE_FIT_FUNCTION_HH

#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "TF1.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class SimpleFitFunction : public FitFunction
{
  public:
    // static const std::vector<std::string> functionList;
    static std::shared_ptr<SimpleFitFunction> createFunctionOfType(FunctionType functionType, const std::string &name,
                                                                  const std::string &expFormula, double min, double max);
    // static std::vector<FitFunction> loadFunctions(const std::string& fileName);
    // static void saveFunctions(std::vector<FitFunction>& functions, const std::string& fileName);
    SimpleFitFunction() = default;
    TF1* getFunction();
    const TF1* getFunction() const;
    void setFunction(const TF1& function, FunctionType funcType);
    double getMin() const;
    double getMax() const;

    std::string getParameter(std::string name) const;

    double evaluate(double x) const;
    double evaluate(double x, const NuisanceValues &nuisances) const;
    double evaluate(double observable, double modelMass,
                    const NuisanceValues &nuisances = {}) const override;
    double evaluateWithParameters(double x, const std::vector<double> &parameters) const;
    std::string getExpression(const std::string &variable) const;
    std::string getNormExpression(const std::string &variable) const override;
    virtual int getNormParameterIndex() const { return -1; }
    virtual bool variesWithSystematic(int parameter) const { return true; }

    void addSystematic(const std::string& sysName, const TF1& upFunction, const TF1& downFunction);
    void addSystematic(const std::string& sysName, const std::vector<double>& upParams, const std::vector<double>& downParams);
    const TF1* getSystematic(const std::string& sysName, bool up) const;
    std::vector<std::string> listSystematics() const override;
    // implement functions into source code
    // modify input output stuff (start with implements)

  protected:
    SimpleFitFunction(const TF1& func, FunctionType funcType);
    explicit SimpleFitFunction(FunctionType funcType);
    virtual void restoreFunction(TF1 &func) const = 0;

  private:
    mutable TF1 function;
    std::map<std::string, std::pair<TF1, TF1>> systematics; 

    static std::vector<std::string> split(const std::string &str, char delimiter);
    ClassDefOverride(SimpleFitFunction, 1)
};

std::ostream &operator<<(std::ostream &stream, const SimpleFitFunction &function);
std::istream &operator>>(std::istream &stream, std::shared_ptr<SimpleFitFunction> &function);

#endif
