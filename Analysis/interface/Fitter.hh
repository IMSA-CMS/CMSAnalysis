#ifndef FITTER_HH
#define FITTER_HH

#include <unordered_map>
#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"

class Fitter
{
public:
	std::string functionFile;
	std::string parameterFile;
	std::unordered_map<std::string, TH1*> histograms;
	FitFunctionCollection functions;

	Fitter();
	void loadFunctions(const FitFunctionCollection& fitFunctions);
	void loadHistogram(const std::unordered_map<std::string, TH1*>& hists);

	void fitFunctions();

	void setFunctionOutput(const std::string& fileName);
	void setParameterOutput(const std::string& fileName);

private:
	TCanvas* fitExpressionFormula(TH1* histogram, FitFunction& fitFunction);
	// TCanvas* fitDSCB(TH1* histogram, FitFunction& fitFunction);
	TCanvas* fitPowerLaw(TH1* histogram, FitFunction& fitFunction);
};

#endif