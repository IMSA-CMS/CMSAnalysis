#ifndef FITTER_HH
#define FITTER_HH

#include <map>
#include <TH1.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
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

	TFitResultPtr fitFunction(TF1* function, TH1* hist);
	TFitResultPtr fitFunction(TF1* function, TGraph* graph);

	void fitFunctions();
	void parameterizeFunctions(std::unordered_map<std::string, double>& xData, const std::vector<std::string>& paramNames);

	const std::unordered_map<std::string, TH1*>& getHistograms() const { return histograms; }
	void setHistograms(const std::unordered_map<std::string, TH1*>& newHistograms) { histograms = newHistograms; }

	const FitFunctionCollection& getFunctions() const { return functions; }
	void setFunctions(const FitFunctionCollection& newFunctions) { functions = newFunctions; }
private:
	TCanvas* fitExpressionFormula(TH1* histogram, FitFunction& fitFunction);
	TCanvas* fitDSCB(TH1* histogram, FitFunction& fitFunction);
	TCanvas* fitPowerLaw(TH1* histogram, FitFunction& fitFunction);
	TCanvas* fitDoubleGaussian(TH1* histogram, FitFunction& fitFunction);
	
	//Insert blank TF1* function ptr which will have the fitted function written to
	static TFitResultPtr functionFittingLoop(TGraph* graph, TF1* function);
	static TFitResultPtr fitSingleFunction(TGraph* histogram, TF1* function, size_t iterations = 1);

	// TH1* readHistogram(const std::string& name);
	std::vector<ParameterizationData> getParameterData(std::unordered_map<std::string, double>& xData);
	FitFunction parameterizeFunction(ParameterizationData& parameterData);

	//Different ways to try and get seed function for fitting
	//Gets guess inverse power law function y = a(x-b)^-c + d using three points, only guarantees the powerlaw through p0 and p1, not p2
	static TF1* seedInversePowerLaw(double x_0, double y_0, double x_1, double y_1, double x_2, double y_2);
	static std::vector<TF1*> trialFitFunctions;

	
	TFile* fitRootFile;
	std::string fitTextFile;

	TFile* parameterRootFile;
	std::string parameterTextFile;

	std::unordered_map<std::string, TH1*> histograms;
	FitFunctionCollection functions;

	std::map<std::string, TDirectory*> fitDirectories;
	std::map<std::string, TDirectory*> parameterDirectories;
};

#endif