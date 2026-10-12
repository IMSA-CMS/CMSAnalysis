#ifndef DOUBLE_GAUSSIAN_FIT_FUNCTION_HH
#define DOUBLE_GAUSSIAN_FIT_FUNCTION_HH

#include "CMSAnalysis/Analysis/interface/SimpleFitFunction.hh"

class DoubleGaussianFitFunction : public SimpleFitFunction
{
  public:
    DoubleGaussianFitFunction();
    DoubleGaussianFitFunction(const std::string &name, double min, double max);

  protected:
    void restoreFunction(TF1 &func) const override;

  private:
    static double evaluateTF1(double *x, double *par);
    ClassDefOverride(DoubleGaussianFitFunction, 1)
};

#endif
