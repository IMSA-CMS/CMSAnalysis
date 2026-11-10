#include "CMSAnalysis/Analysis/interface/DSCBFitFunction.hh"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iomanip>
#include <sstream>

ClassImp(DSCBFitFunction)

DSCBFitFunction::DSCBFitFunction()
    : SimpleFitFunction(FunctionType::DoubleSidedCrystalBall)
{
}

DSCBFitFunction::DSCBFitFunction(const std::string &name, double min, double max)
    : SimpleFitFunction(TF1(name.data(), evaluateTF1, min, max, 7, 1, TF1::EAddToList::kNo),
                        FunctionType::DoubleSidedCrystalBall)
{
    getFunction()->SetParNames("#alpha_{low}", "#alpha_{high}", "n_{low}", "n_{high}", "#mu", "#sigma", "norm");
    getFunction()->SetParameters(1.0, 1.0, 2.0, 2.0, 0.0, 1.0, 0.0);
}

double DSCBFitFunction::evaluateTF1(double *x, double *par)
{
    for (int i = 0; i < 7; ++i)
    {
        if (!std::isfinite(par[i]))
            throw std::domain_error("DSCB parameters must be finite");
    }
    if (!std::isfinite(x[0]) || par[0] <= 0 || par[1] <= 0 || par[5] <= 0 || par[6] < 0)
        throw std::domain_error("DSCB requires positive transitions and width, and nonnegative normalization");

    const double alpha_l = par[0];
    const double alpha_h = par[1];
    const double n_l = std::max(par[2], 1.000001);
    const double n_h = std::max(par[3], 1.000001);
    const double t = (x[0] - par[4]) / par[5];
    if (!std::isfinite(t))
        throw std::domain_error("DSCB standardized coordinate is not finite");

    double logShape = -0.5 * t * t;
    if (t < -alpha_l)
        logShape = -0.5 * alpha_l * alpha_l - n_l * std::log1p(alpha_l * (-alpha_l - t) / n_l);
    else if (t > alpha_h)
        logShape = -0.5 * alpha_h * alpha_h - n_h * std::log1p(alpha_h * (t - alpha_h) / n_h);

    const double lowTailNorm = (n_l / (n_l - 1)) / alpha_l * std::exp(-0.5 * alpha_l * alpha_l);
    const double highTailNorm = (n_h / (n_h - 1)) / alpha_h * std::exp(-0.5 * alpha_h * alpha_h);
    const double gaussianNorm = std::sqrt(M_PI / 2) *
        (std::erf(alpha_l / std::sqrt(2.0)) + std::erf(alpha_h / std::sqrt(2.0)));
    const double integral = gaussianNorm + lowTailNorm + highTailNorm;
    if (!std::isfinite(integral) || integral <= 0)
        throw std::domain_error("DSCB normalization is not finite and positive");
    const double value = par[6] / par[5] / integral * std::exp(logShape);
    if (!std::isfinite(value) || value < 0)
        throw std::domain_error("DSCB density is not finite and nonnegative");
    return value;
}

void DSCBFitFunction::restoreFunction(TF1 &func) const
{
    func.SetFunction(evaluateTF1);
}

std::string DSCBFitFunction::getNormExpression(const std::string &) const
{
    std::ostringstream expression;
    expression << std::setprecision(17) << getFunction()->GetParameter(6);
    return expression.str();
}
