#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include <TStyle.h>
#include <TH1.h>
#include <TCanvas.h>

Fitter::Fitter() {}

void Fitter::loadFunctions(const FitFunctionCollection& fitFunctions)
{
	functions = fitFunctions;
}

void Fitter::loadHistogram(const std::unordered_map<std::string, TH1*>& hists)
{
	histograms = hists;
}

void Fitter::fitFunctions()
{
	TFile* fitFile = TFile::Open(functionFile.c_str(), "RECREATE");
	for (auto& funcPair : functions.functions)
	{
		FitFunction& func = funcPair.second;
		TCanvas* canvas;
		switch (func.getFunctionType())
		{
			case FitFunction::FunctionType::EXPRESSION_FORMULA:
				canvas = fitExpressionFormula(histograms[funcPair.first], func);
				break;
			default:
				throw std::invalid_argument("Not a valid FunctionType enum value");
		}

		fitFile->WriteObject(canvas, func.getName().c_str());
	}

	functions.saveFunctions(parameterFile);
}

void Fitter::setFunctionOutput(const std::string& fileName)
{
	functionFile = fileName;
}

void Fitter::setParameterOutput(const std::string& fileName)
{
	parameterFile = fileName;
}

TCanvas* Fitter::fitExpressionFormula(TH1* histogram, FitFunction& fitFunction)
{
	TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
	c1->SetLogy();

	TFitResultPtr result = histogram->Fit(fitFunction.getFunction(), "SL", "", fitFunction.getMin(), fitFunction.getMax());

	gStyle->SetOptFit(1111);

	return c1;
}
// TCanvas* fitDSCB(TH1* histogram, FitFunction& fitFunction);
TCanvas* Fitter::fitPowerLaw(TH1* histogram, FitFunction& fitFunction)
{
	TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
	c1->SetLogy();

	TFitResultPtr result = histogram->Fit(fitFunction.getFunction(), "SL", "", fitFunction.getMin(), fitFunction.getMax());

	// double chi2 = __DBL_MAX__;
	// while (result->Chi2() < chi2)
	// {
	// 	chi2 = result->Chi2();
	// 	f1->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
	// 	result = hist->Fit("powerLaw", "SL", "", lowerBound, upperBound);
	// }

	gStyle->SetOptFit(1111);

	return c1;
}