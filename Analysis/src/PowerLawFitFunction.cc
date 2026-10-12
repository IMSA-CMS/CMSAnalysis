#include "CMSAnalysis/Analysis/interface/PowerLawFitFunction.hh"
#include <cmath>
#include <iomanip>
#include <sstream>

ClassImp(PowerLawFitFunction)

PowerLawFitFunction::PowerLawFitFunction()
    : SimpleFitFunction(FunctionType::PowerLaw)
{
}

PowerLawFitFunction::PowerLawFitFunction(const std::string &name, double min, double max)
    : SimpleFitFunction(TF1(name.data(), evaluateTF1, min, max, 3, 1, TF1::EAddToList::kNo),
                        FunctionType::PowerLaw)
{
}

double PowerLawFitFunction::evaluateTF1(double *x, double *par)
{
    return par[0] * pow(x[0] - par[1], par[2]);
}

void PowerLawFitFunction::restoreFunction(TF1 &func) const
{
    func.SetFunction(evaluateTF1);
}

std::string PowerLawFitFunction::getNormExpression(const std::string &) const
{
    const auto *func = getFunction();
    const double coeff = func->GetParameter(0);
    const double shift = func->GetParameter(1);
    const double expo = func->GetParameter(2);
    const double lower = getMin() - shift;
    const double upper = getMax() - shift;

    double norm;
    if (expo != -1)
    {
        norm = coeff / (expo + 1) * (std::pow(upper, expo + 1) - std::pow(lower, expo + 1));
    }
    else
    {
        norm = coeff * std::log(upper / lower);
    }
    std::ostringstream expression;
    expression << std::setprecision(17) << norm;
    return expression.str();
}
