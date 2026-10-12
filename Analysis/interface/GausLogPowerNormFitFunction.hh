#ifndef GAUS_LOG_POWER_NORM_FIT_FUNCTION_HH
#define GAUS_LOG_POWER_NORM_FIT_FUNCTION_HH

#include "CMSAnalysis/Analysis/interface/SimpleFitFunction.hh"

class GausLogPowerNormFitFunction : public SimpleFitFunction
{
  public:
    GausLogPowerNormFitFunction();
    GausLogPowerNormFitFunction(const std::string &name, double min, double max);
    int getNormParameterIndex() const override { return 0; }

  protected:
    void restoreFunction(TF1 &func) const override;

  private:
    static double evaluateTF1(double *x, double *par);
    ClassDefOverride(GausLogPowerNormFitFunction, 1)
};

#endif
