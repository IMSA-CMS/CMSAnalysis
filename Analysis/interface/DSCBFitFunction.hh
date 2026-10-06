#ifndef DSCB_FIT_FUNCTION_HH
#define DSCB_FIT_FUNCTION_HH

#include "CMSAnalysis/Analysis/interface/SimpleFitFunction.hh"

class DSCBFitFunction : public SimpleFitFunction
{
  public:
    DSCBFitFunction();
    DSCBFitFunction(const std::string &name, double min, double max);
    std::string getNormExpression(const std::string &variable) const override;
    int getNormParameterIndex() const override { return 6; }
    bool variesWithSystematic(int parameter) const override { return parameter != 6; }

  protected:
    void restoreFunction(TF1 &func) const override;

  private:
    static double evaluateTF1(double *x, double *par);
    ClassDefOverride(DSCBFitFunction, 1)
};

#endif
