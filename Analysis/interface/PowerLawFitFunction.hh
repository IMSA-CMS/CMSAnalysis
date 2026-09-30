#ifndef POWER_LAW_FIT_FUNCTION_HH
#define POWER_LAW_FIT_FUNCTION_HH

#include "CMSAnalysis/Analysis/interface/SimpleFitFunction.hh"

class PowerLawFitFunction : public SimpleFitFunction
{
  public:
    PowerLawFitFunction();
    PowerLawFitFunction(const std::string &name, double min, double max);
    std::string getNormExpression(const std::string &variable) const override;

  protected:
    void restoreFunction(TF1 &func) const override;

  private:
    static double evaluateTF1(double *x, double *par);
    ClassDefOverride(PowerLawFitFunction, 1)
};

#endif
