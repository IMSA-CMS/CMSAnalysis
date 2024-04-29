#ifndef FITTER_HH
#define FITTER_HH

#include <unordered_map>
#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"


struct ParameterizationData
{
	std::vector<double> x;
 	std::vector<double> y;
	std::vector<double> error;
	std::vector<double> zero;
	int size;
	std::string name;
};

class Fitter
{
public:
	// TFile* histogramFile;
	TFile* fitOutputFile;
	std::string fitParameterFile;

	TFile* parameterRootFile;
	std::string parameterizationFunctionFile;

	std::unordered_map<std::string, TH1*> histograms;
	FitFunctionCollection functions;
	
	Fitter();
	Fitter(const std::string& functionFile, const std::string& fitParameterFile);
	Fitter(const std::string& functionFile, const std::string& fitParameterFile, const std::string& parameterRootFile, const std::string& parameterizationFuncFile);
	~Fitter();

	void setFunctionRootOutput(const std::string& name);
	void setFunctionOutput(const std::string& name);
	void setParameterizationRootOutput(const std::string& name);
	void setParameterizationOutput(const std::string& name);

	
	void loadFunctions(const FitFunctionCollection& fitFunctions);
	// void loadHistogram(const std::vector<string>& histNames);

	void fitFunctions();
	void parameterizeFunctions(std::unordered_map<std::string, double>& xData, const std::vector<std::string>& paramNames);
private:
	TCanvas* fitExpressionFormula(TH1* histogram, FitFunction& fitFunction);
	TCanvas* fitDSCB(TH1* histogram, FitFunction& fitFunction);
	TCanvas* fitPowerLaw(TH1* histogram, FitFunction& fitFunction);

	// TH1* readHistogram(const std::string& name);
	std::vector<ParameterizationData> getParameterData(std::unordered_map<std::string, double>& xData);
	FitFunction parameterizeFunction(ParameterizationData& parameterData);
};

#endif