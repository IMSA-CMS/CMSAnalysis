#include "CMSAnalysis/Analysis/interface/ExpressionFitFunction.hh"

ClassImp(ExpressionFitFunction)

ExpressionFitFunction::ExpressionFitFunction()
    : SimpleFitFunction(FunctionType::ExpressionFormula)
{
}

ExpressionFitFunction::ExpressionFitFunction(const std::string &name, const std::string &formula, double min, double max)
    : SimpleFitFunction(TF1(name.data(), formula.data(), min, max, TF1::EAddToList::kNo),
                        FunctionType::ExpressionFormula)
{
}

void ExpressionFitFunction::restoreFunction(TF1 &) const
{
}
