#include "CMSAnalysis/Analysis/interface/DSCBFitFunction.hh"
#include <cmath>
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
}

double DSCBFitFunction::evaluateTF1(double *x, double *par)
{
    const double alpha_l = par[0];
    const double alpha_h = par[1];
    const double n_l = par[2];
    const double n_h = par[3];
    const double mean = par[4];
    const double sigma = par[5];
    const double N = par[6];
    // we need a double for adequate precision here, cannot be a float
    const double t = (x[0] - mean) / sigma;
    double result;
    const double fact1TLessMinosAlphaL = alpha_l / n_l;
    const double fact2TLessMinosAlphaL = (n_l / alpha_l) - alpha_l - t;
    const double fact1THihgerAlphaH = alpha_h / n_h;
    const double fact2THigherAlphaH = (n_h / alpha_h) - alpha_h + t;

    const double root2 = std::pow(2, 0.5);
    if (-alpha_l <= t && alpha_h >= t)
    {
        result = exp(-0.5 * t * t);
    }
    else if (t < -alpha_l)
    {
        result = exp(-0.5 * alpha_l * alpha_l) * pow(fact1TLessMinosAlphaL * fact2TLessMinosAlphaL, -n_l);
    }
    else
    {
        result = exp(-0.5 * alpha_h * alpha_h) * pow(fact1THihgerAlphaH * fact2THigherAlphaH, -n_h);
    }

    const double lowTailNorm = (n_l / std::abs(alpha_l)) / (n_l - 1) * std::exp(-0.5 * alpha_l * alpha_l);
    const double highTailNorm = (n_h / std::abs(alpha_h)) / (n_h - 1) * std::exp(-0.5 * alpha_h * alpha_h);
    const double gaussianNormA = erf(std::abs(alpha_l / root2)) + erf(std::abs(alpha_h / root2));
    const double gaussianNormB = std::pow(M_PI / 2, 0.5) * gaussianNormA;
    const double functionNormalization = std::pow(sigma * (gaussianNormB + lowTailNorm + highTailNorm), -1);
    return N * functionNormalization * result;
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
