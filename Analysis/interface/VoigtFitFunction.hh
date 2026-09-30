#ifndef VOIGT_FIT_FUNCTION_HH
#define VOIGT_FIT_FUNCTION_HH

#include "CMSAnalysis/Analysis/interface/SimpleFitFunction.hh"

class VoigtFitFunction : public SimpleFitFunction
{
  public:
    VoigtFitFunction();
    VoigtFitFunction(const std::string &name, double min, double max);
    std::string getNormExpression(const std::string &variable) const override;
    int getNormParameterIndex() const override { return 0; }

  protected:
    void restoreFunction(TF1 &func) const override;

  private:
    static double evaluateTF1(double *x, double *par);
    ClassDefOverride(VoigtFitFunction, 1)
};

#endif
