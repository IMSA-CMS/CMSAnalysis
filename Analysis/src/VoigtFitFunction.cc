#include "CMSAnalysis/Analysis/interface/VoigtFitFunction.hh"
#include <TMath.h>
#include <iomanip>
#include <sstream>

ClassImp(VoigtFitFunction)

VoigtFitFunction::VoigtFitFunction()
    : SimpleFitFunction(FunctionType::Voigt)
{
}

VoigtFitFunction::VoigtFitFunction(const std::string &name, double min, double max)
    : SimpleFitFunction(TF1(name.data(), evaluateTF1, min, max, 4, 1, TF1::EAddToList::kNo),
                        FunctionType::Voigt)
{
    getFunction()->SetParNames("N", "#mu", "#Gamma", "#sigma");
}

double VoigtFitFunction::evaluateTF1(double *x, double *par)
{
    return par[0] * TMath::Voigt(x[0] - par[1], par[3], par[2]);
}

void VoigtFitFunction::restoreFunction(TF1 &func) const
{
    func.SetFunction(evaluateTF1);
}

std::string VoigtFitFunction::getNormExpression(const std::string &) const
{
    std::ostringstream expression;
    expression << std::setprecision(17) << getFunction()->GetParameter(0);
    return expression.str();
}
