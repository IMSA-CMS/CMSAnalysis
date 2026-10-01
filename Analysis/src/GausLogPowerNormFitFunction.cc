#include "CMSAnalysis/Analysis/interface/GausLogPowerNormFitFunction.hh"
#include <cmath>

ClassImp(GausLogPowerNormFitFunction)

GausLogPowerNormFitFunction::GausLogPowerNormFitFunction()
    : SimpleFitFunction(FunctionType::GausLogPowerNorm)
{
}

GausLogPowerNormFitFunction::GausLogPowerNormFitFunction(const std::string &name, double min, double max)
    : SimpleFitFunction(TF1(name.data(), evaluateTF1, min, max, 5, 1, TF1::EAddToList::kNo),
                        FunctionType::GausLogPowerNorm)
{
    getFunction()->SetParNames("N", "#mu", "#sigma_{1}", "s", "n");
}

double GausLogPowerNormFitFunction::evaluateTF1(double *xs, double *par)
{
    const auto x = xs[0];
    const auto mult = par[0];
    const auto u = par[1];
    const auto sigma1 = par[2];
    const auto s = par[3];
    const auto n = par[4];

    if (x <= u)
    {
        return mult * exp(-(x - u) * (x - u) / (2 * sigma1 * sigma1));
    }
    return mult * exp(-s * pow(log(x / u), n));
}

void GausLogPowerNormFitFunction::restoreFunction(TF1 &func) const
{
    func.SetFunction(evaluateTF1);
}
