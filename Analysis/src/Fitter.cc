#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include <TStyle.h>
#include <TH1.h>
#include <TFitResult.h>
#include <TCanvas.h>

Fitter::Fitter(const std::string& histogramFile, const std::string& functionFile, const std::string& parameterFile) 
	: histogramFile(TFile::Open(histogramFile.c_str())), 
	fitOutputFile(TFile::Open(functionFile.c_str(), "RECREATE")), 
	parameterFile(parameterFile) {}

Fitter::~Fitter() 
{
	histogramFile->Close();
	fitOutputFile->Close();
}


void Fitter::loadFunctions(const FitFunctionCollection& fitFunctions)
{
	functions = fitFunctions;
}

// void Fitter::loadHistogram(const std::vector<std::string>& histNames)
// {
// 	histograms = histNames;
// }

void Fitter::fitFunctions()
{
	for (auto& funcPair : functions.functions)
	{
		FitFunction& func = funcPair.second;
		TH1* histogram = readHistogram(funcPair.first);
		if (histogram == nullptr)
		{
			std::cout << "Could not find histogram " << funcPair.first << '\n';
			continue;
		}
		TCanvas* canvas;

		switch (func.getFunctionType())
		{
			case FitFunction::FunctionType::EXPRESSION_FORMULA:
				canvas = fitExpressionFormula(histogram, func);
				break;
			case FitFunction::FunctionType::DOUBLE_SIDED_CRYSTAL_BALL:
				canvas = fitDSCB(histogram, func);
				break;
			case FitFunction::FunctionType::POWER_LAW:
				canvas = fitPowerLaw(histogram, func);
				break;
			default:
				throw std::invalid_argument("Not a valid FunctionType enum value");
		}

		fitOutputFile->WriteObject(canvas, func.getName().c_str());
	}

	functions.saveFunctions(parameterFile);
}

void Fitter::setFunctionOutput(const std::string& fileName)
{
	fitOutputFile = TFile::Open(fileName.c_str(), "RECREATE");
}

void Fitter::setParameterOutput(const std::string& fileName)
{
	parameterFile = fileName;
}

TH1* Fitter::readHistogram(const std::string& name)
{
	return dynamic_cast<TH1*>(histogramFile->FindObjectAny(name.c_str()));
}

TCanvas* Fitter::fitExpressionFormula(TH1* histogram, FitFunction& fitFunction)
{
	TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
	c1->SetLogy();

	TFitResultPtr result = histogram->Fit(fitFunction.getFunction(), "SL", "", fitFunction.getMin(), fitFunction.getMax());

	gStyle->SetOptFit(1111);

	return c1;
}
TCanvas* Fitter::fitDSCB(TH1* histogram, FitFunction& fitFunction)
{
	return new TCanvas();
}
TCanvas* Fitter::fitPowerLaw(TH1* histogram, FitFunction& fitFunction)
{
	double initalParams[3] = {std::pow(10, 17), -400, -7};
	fitFunction.getFunction()->SetParameters(initalParams);

	TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
	c1->SetLogy();

	TFitResultPtr result = histogram->Fit(fitFunction.getFunction(), "SL", "", fitFunction.getMin(), fitFunction.getMax());

	double chi2 = __DBL_MAX__;
	while (result->Chi2() < chi2)
	{
		chi2 = result->Chi2();
		fitFunction.getFunction()->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
		result = histogram->Fit(fitFunction.getFunction(), "SL", "", 150, 1500);
	}

	gStyle->SetOptFit(1111);

	return c1;
}