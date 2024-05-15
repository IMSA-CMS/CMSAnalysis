#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>
#include <map>
#include <float.h>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TFormula.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#define _USE_MATH_DEFINES

std::vector<std::string> channelTypes =
{
	"eeee",
	"eeeu",
	"eueu",
	"eeuu",
	"euuu",
	"uuuu"
};

std::vector<std::string> histogramTypes = 
{
	"Same Sign Invariant Mass"
};

// std::vector<std::string> channelTypes =
// {
// 	"eeee",
// 	"eeeu",
// 	"eueu",
// 	"eeuu",
// 	"euuu",
// 	"uuuu",
// 	"eee",
// 	"eeu",
// 	"eue",
// 	"euu",
// 	"uue",
// 	"uuu"
// };

// std::vector<std::string> histogramTypes = 
// {
// 	"Same Sign Invariant Mass",
// 	"Reco Invariant Mass Background"
// };

// Fits multiple graphs with the same function
void multipleFits();

TH1* getHist(std::string name, TFile* file);
bool checkToProject(TH1* histogram);
TH1* getPositiveLeptonHist(TH2* histogram);
TH1* getNegativeLeptonHist(TH2* histogram);
bool writeHistogramFit(const TFitResultPtr& fitResults, std::fstream& parameterFile);
int getLowestDerivativeBin(TH1* histogram);
std::vector<double> randomizeParameters(const std::vector<double>& parameters);

std::vector<double> getInitialPowerLawParams(TH1* histogram, double lowerBin, double upperBin);

double powerLaw(double *x, double *par);
double exponential(double *x, double *par);

TFitResultPtr fitToPowerLaw(char const* name, TH1* hist, TFile* file, std::vector<double> params, double lowerBound, double upperBound);
TFitResultPtr fitToExponential(char const* name, TH1* hist, TFile* file, std::vector<double> params, double lowerBound, double upperBound);
TFitResultPtr fitToPowerLaw(char const* name, TH1* hist, TFile* file, std::vector<double> params, double lowerBound, double upperBound, int iterations);
TFitResultPtr fitToExponential(char const* name, TH1* hist, TFile* file, std::vector<double> params, double lowerBound, double upperBound, int iterationss);

void fullSignalFit();

void backgroundFit()
{
	


	// // std::cout << f1.GetNpar() << '\n';

	// FitFunctionCollection functions;
	// FitFunction f2 = FitFunction::createFunctionOfType(FitFunction::FunctionType::POWER_LAW,
	// "test", "", 150, 1500);
	// TF1* func = f2.getFunction();

	// func->SetParNames("testOne", "testTwo", "testThree");
	// func->SetParameters(3, 2, 1);
	// double array[3] = {1, 2, 3};
	// func->SetParErrors(array);

	// // std::cout << f2;
	// functions.insert(f2);
	// // std::cout << functions[0];
	// functions.saveFunctions("testFunctions.txt");	
	
	// auto funcs = FitFunctionCollection::loadFunctions("testFunctions.txt");

	// std::cout << funcs["test"];
	// multipleFits();
	// auto paramData = getParams();
	// graph(paramData);
	//	fitParameters();

	fullSignalFit();
}

void fullSignalFit() 
{
	const double min = 0;
	const double max = 2000;

	std::string fitHistsName = "histogramFits.root";
	std::string fitParameterValueFile = "histogramFunctions.txt";
	std::string parameterFits = "parameterFits.root";
	std::string parameterFunctions = "parameterFunctions.txt";
	std::vector<int> masses = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};

	Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);

    std::shared_ptr<HiggsCompleteAnalysis> analysis = std::make_shared<HiggsCompleteAnalysis>();
    std::cout << "Loaded histogram\n";
    std::unordered_map<std::string, double> massValues;
    std::unordered_map<std::string, TH1*> histogramMap;
    FitFunctionCollection currentFunctions;
    std::vector<std::string> paramNames = {"alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma", "norm"};

	for (const auto& histType : histogramTypes) 
	{
		for (const auto& channel : channelTypes) 
		{
            auto targetChannel = analysis->getChannel(channel);

			for (size_t i = 0; i < masses.size(); ++i) 
			{
                auto process = targetChannel->findProcess("Higgs Signal " + std::to_string(masses[i]));
				TH1* selectedHist = process->getHist("Same Sign Inv Mass", true);
                std::cout << "Got Hist\n";
                std::string keyName = std::to_string(masses[i]) + '_' + channel + '_' + histType;

				FitFunction func = FitFunction::createFunctionOfType(FitFunction::DOUBLE_SIDED_CRYSTAL_BALL, keyName, "", min, max);
				currentFunctions.insert(func);
				histogramMap.insert({keyName, selectedHist});
				massValues.insert({keyName, masses[i]});

				// file->Close();
				// selectedHist->Draw();
				// std::string wait;
				// std::cin >> wait;
			}
		}
        
	}
    fitter.histograms = histogramMap;
    fitter.loadFunctions(currentFunctions);
    fitter.fitFunctions();
    fitter.parameterizeFunctions(massValues, paramNames);

}
