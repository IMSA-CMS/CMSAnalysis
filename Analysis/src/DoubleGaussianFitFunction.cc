#include "CMSAnalysis/Analysis/interface/DoubleGaussianFitFunction.hh"
#include <TMath.h>

ClassImp(DoubleGaussianFitFunction)

DoubleGaussianFitFunction::DoubleGaussianFitFunction()
    : SimpleFitFunction(FunctionType::DoubleGaussian)
{
}

DoubleGaussianFitFunction::DoubleGaussianFitFunction(const std::string &name, double min, double max)
    : SimpleFitFunction(TF1(name.data(), evaluateTF1, min, max, 6, 1, TF1::EAddToList::kNo),
                        FunctionType::DoubleGaussian)
{
    getFunction()->SetParNames("mul_{1}", "#mu_{1}", "#sigma_{1}", "mul_{2}", "#mu_{2}", "#sigma_{2}");
}

double DoubleGaussianFitFunction::evaluateTF1(double *x, double *par)
{
    return par[0] * TMath::Gaus(x[0], par[1], par[2]) + par[3] * TMath::Gaus(x[0], par[4], par[5]);
}

void DoubleGaussianFitFunction::restoreFunction(TF1 &func) const
{
    func.SetFunction(evaluateTF1);
}
