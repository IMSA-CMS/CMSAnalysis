#include "CMSAnalysis/Analysis/interface/Fitter.hh"

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
	return new TCanvas();
}
// TCanvas* fitDSCB(TH1* histogram, FitFunction& fitFunction);
TCanvas* Fitter::fitPowerLaw(TH1* histogram, FitFunction& fitFunction)
{
	return new TCanvas();
}