#include "CMSAnalysis/Analysis/interface/DSCBFitFunction.hh"
#include <algorithm>
#include <array>
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
    struct Constants
    {
        std::array<double, 5> parameters{};
        double lowScale = 0, highScale = 0;
        double lowOffset = 0, highOffset = 0;
        double lowExp = 0, highExp = 0;
        double normalization = 0;
        bool valid = false;
    };
    // TF1 callbacks have no instance state. Keep one shape per thread and
    // check its inputs so different functions and systematic variations cannot reuse stale constants.
    static thread_local Constants constants;
    const std::array<double, 5> parameters{alpha_l, alpha_h, n_l, n_h, sigma};
    if (!constants.valid || constants.parameters != parameters)
    {
        constants.lowScale = alpha_l / n_l;
        constants.highScale = alpha_h / n_h;
        constants.lowOffset = (n_l / alpha_l) - alpha_l;
        constants.highOffset = (n_h / alpha_h) - alpha_h;
        constants.lowExp = std::exp(-0.5 * alpha_l * alpha_l);
        constants.highExp = std::exp(-0.5 * alpha_h * alpha_h);
        const double root2 = std::pow(2, 0.5);
        const double lowTailNorm = (n_l / std::abs(alpha_l)) / (n_l - 1) * constants.lowExp;
        const double highTailNorm = (n_h / std::abs(alpha_h)) / (n_h - 1) * constants.highExp;
        const double gaussianNormA = erf(std::abs(alpha_l / root2)) + erf(std::abs(alpha_h / root2));
        const double gaussianNormB = std::pow(M_PI / 2, 0.5) * gaussianNormA;
        constants.normalization = std::pow(sigma * (gaussianNormB + lowTailNorm + highTailNorm), -1);
        constants.parameters = parameters;
        constants.valid = true;
    }
    // we need a double for adequate precision here, cannot be a float
    const double t = (x[0] - mean) / sigma;
    double result;
    if (-alpha_l <= t && alpha_h >= t)
    {
        result = exp(-0.5 * t * t);
    }
    else if (t < -alpha_l)
    {
        result = constants.lowExp * pow(constants.lowScale * (constants.lowOffset - t), -n_l);
    }
    else
    {
        result = constants.highExp * pow(constants.highScale * (constants.highOffset + t), -n_h);
    }

    return N * constants.normalization * result;
}

void DSCBFitFunction::restoreFunction(TF1 &func) const
{
    func.SetFunction(evaluateTF1);
}

double DSCBFitFunction::integralWithParameters(double low, double high,
                                               const std::vector<double> &parameters) const
{
    if (parameters.size() != 7)
        throw std::invalid_argument("DSCB requires seven parameters");
    if (low > high)
        return -integralWithParameters(high, low, parameters);
    const double alphaL = parameters[0], alphaR = parameters[1];
    const double nL = parameters[2], nR = parameters[3];
    const double mean = parameters[4], sigma = parameters[5];
    const double tLow = (low - mean) / sigma, tHigh = (high - mean) / sigma;

    // Integrate each tail in distance from its transition. expm1 avoids
    // cancellation when the exponent is close to one or the interval is small.
    const auto tail = [](double near, double far, double alpha, double n) {
        const double scale = n / alpha;
        const double logNear = std::log1p((near - alpha) / scale);
        const double logRatio = std::log1p((far - near) / (scale + near - alpha));
        const double exponent = 1.0 - n;
        const double powerIntegral = exponent == 0.0 ? logRatio :
            std::exp(exponent * logNear) * std::expm1(exponent * logRatio) / exponent;
        return std::exp(-0.5 * alpha * alpha) * scale * powerIntegral;
    };

    double area = 0.0;
    if (tLow < -alphaL)
        area += tail(-std::min(tHigh, -alphaL), -tLow, alphaL, nL);
    const double coreLow = std::max(tLow, -alphaL);
    const double coreHigh = std::min(tHigh, alphaR);
    const double root2 = std::sqrt(2.0);
    if (coreLow < coreHigh)
    {
        const double gaussianArea = coreLow >= 0.0 ?
            std::erfc(coreLow / root2) - std::erfc(coreHigh / root2) :
            (coreHigh <= 0.0 ? std::erfc(-coreHigh / root2) - std::erfc(-coreLow / root2) :
                              std::erf(coreHigh / root2) - std::erf(coreLow / root2));
        area += std::sqrt(M_PI / 2.0) * gaussianArea;
    }
    if (tHigh > alphaR)
        area += tail(std::max(tLow, alphaR), tHigh, alphaR, nR);

    // Match the existing evaluateTF1 normalization, including its yield factor.
    const double lowTailNorm = (nL / std::abs(alphaL)) / (nL - 1) * std::exp(-0.5 * alphaL * alphaL);
    const double highTailNorm = (nR / std::abs(alphaR)) / (nR - 1) * std::exp(-0.5 * alphaR * alphaR);
    const double coreNorm = std::sqrt(M_PI / 2.0) *
        (std::erf(std::abs(alphaL / root2)) + std::erf(std::abs(alphaR / root2)));
    return parameters[6] * area / (coreNorm + lowTailNorm + highTailNorm);
}

std::string DSCBFitFunction::getNormExpression(const std::string &) const
{
    std::ostringstream expression;
    expression << std::setprecision(17) << getFunction()->GetParameter(6);
    return expression.str();
}
