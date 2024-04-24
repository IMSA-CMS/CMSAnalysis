#ifndef FITTER_HH
#define FITTER_HH

#include <unordered_map>
#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"


struct ParameterizationData
{
	double* x;
 	double* y;
	double* error;
	double* zero;
	int size;
	std::string name;
};

class Fitter
{
public:
	TFile* histogramFile;
	TFile* fitOutputFile;
	std::string parameterFile;
	FitFunctionCollection functions;

	Fitter(const std::string& histogramFile, const std::string& functionFile, const std::string& parameterFile);
	~Fitter();
	
	void loadFunctions(const FitFunctionCollection& fitFunctions);
	// void loadHistogram(const std::vector<string>& histNames);

	void fitFunctions();

	void setFunctionOutput(const std::string& fileName);
	void setParameterOutput(const std::string& fileName);
	FitFunctionCollection parameterizeFunctions(std::unordered_map<std::string, double>& xData);
private:
	TCanvas* fitExpressionFormula(TH1* histogram, FitFunction& fitFunction);
	TCanvas* fitDSCB(TH1* histogram, FitFunction& fitFunction);
	TCanvas* fitPowerLaw(TH1* histogram, FitFunction& fitFunction);

	TH1* readHistogram(const std::string& name);
	std::unique_ptr<std::vector<ParameterizationData>> getParameterData(std::unordered_map<std::string, double>& xData);
	FitFunction parameterizeFunction(ParameterizationData& parameterData);
};

#endif