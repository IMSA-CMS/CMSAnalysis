#ifndef EXPRESSION_FIT_FUNCTION_HH
#define EXPRESSION_FIT_FUNCTION_HH

#include "CMSAnalysis/Analysis/interface/SimpleFitFunction.hh"

class ExpressionFitFunction : public SimpleFitFunction
{
  public:
    ExpressionFitFunction();
    ExpressionFitFunction(const std::string &name, const std::string &formula, double min, double max);

  protected:
    void restoreFunction(TF1 &func) const override;

  private:
    ClassDefOverride(ExpressionFitFunction, 1)
};

#endif
