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
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"

#define _USE_MATH_DEFINES


double pearson(double *x, double *par)
{
	double a = par[0];
	double m = par[1];
	double mass = par[2];
	double n = par[3];
	double result;

	double constant = n * std::tgamma(m) / (a * std::sqrt(M_PI) * std::tgamma(m - 0.5));
	result = constant * std::pow((1 + ((x[0]-mass)/a) * ((x[0]-mass)/a)), -m);

	return result;
}

std::vector<std::string> channelTypes =
{
	"eeee",
	"eeeu",
	"eueu",
	"eeuu",
	"euuu",
	"uuuu",
	"eee",
	"eeu",
	"eue",
	"euu",
	"uue",
	"uuu"
};

std::vector<std::string> histogramTypes = 
{
	"Same Sign Invariant Mass",
	"Reco Invariant Mass Background"
};

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

void backgroundFit()
{
	
	auto f1 = new TF1("pearsonTest", FitFunction::pearson, 0, 2000, 4);


	// // std::cout << f1.GetNpar() << '\n';

	FitFunctionCollection functions;
	FitFunction f2(f1, FitFunction::FunctionType::DOUBLE_SIDED_CRYSTAL_BALL);
	// std::cout << f2;
	functions.insert(f2);
	// std::cout << functions[0];
	functions.saveFunctions("testFunctions.txt");	
	
	auto funcs = FitFunctionCollection::loadFunctions("testFunctions.txt");

	std::cout << funcs["pearsonTest"];
	// multipleFits();
	// auto paramData = getParams();
	// graph(paramData);
	//	fitParameters();
}

void multipleFits()
{
	const double lowerBound = 150;
	const double upperBound = 1000;
	// Filename of root files that have the graphs to fit in them
	std::string fileName = "ZZ_Decay_4L_Run_2.root";

	// This script creates three root files:
	// The name of the root file the fitted histograms are to be stored
	std::string fitHistsName = "fitHistograms.root";
	std::string fitParameterValueFile = "ZZ_Decay_4L_Fit_Parameters.txt";

	TFile* histFile = TFile::Open(fileName.c_str());
	TFile* fitFile = TFile::Open(fitHistsName.c_str(), "RECREATE");
	std::fstream parameterFile;

	if (!parameterFile.is_open())
	{
		parameterFile.open(fitParameterValueFile, std::ios_base::app);
	}

	for (auto histType : histogramTypes)
	{
		for (auto channel : channelTypes)
		{
			std::string histogramName = channel + "_" + histType;
			TH1* currentHist = getHist(histogramName.c_str(), histFile);

			if (currentHist == nullptr)
			{
				continue;
			}

			if (checkToProject(currentHist))
			{
				TH1* positiveHist = getPositiveLeptonHist((TH2*) currentHist);
				TH1* negativeHist = getNegativeLeptonHist((TH2*) currentHist);

				auto positiveParams = getInitialPowerLawParams(positiveHist, lowerBound, upperBound);
				auto negativeParams = getInitialPowerLawParams(negativeHist, lowerBound, upperBound);

				auto positiveFit = fitToPowerLaw(positiveHist->GetName(), positiveHist, fitFile, positiveParams, lowerBound, 3000);
				auto negativeFit = fitToPowerLaw(negativeHist->GetName(), negativeHist, fitFile, negativeParams, lowerBound, 3000);

				writeHistogramFit(positiveFit, parameterFile);
				writeHistogramFit(negativeFit, parameterFile);
			}
			else
			{
				auto params = getInitialPowerLawParams(currentHist, 200, 1000);
				auto fit = fitToPowerLaw(currentHist->GetName(), currentHist, fitFile, params, lowerBound, 3000);
				writeHistogramFit(fit, parameterFile);
			}
		}
	}

	parameterFile.close();
}

double powerLaw(double *x, double *par)
{
	return par[0] * pow(x[0] - par[1], par[2]);
}

double exponential(double * x, double *par)
{
	return par[0] * pow(par[1], x[0] - par[2]);
}

// std::vector<double> getInitialPowerLawParams(TH1* histogram, double lowerBound, double upperBound)
// {
// 	const double zeroAlt = 0.1;
// 	std::vector<double> startingParameters = {0, 0, -1};
// 	// int bin = getLowestDerivativeBin(histogram);
// 	int lowerBin = lowerBound / histogram->GetBinWidth(1);
// 	int upperBin = upperBound / histogram->GetBinWidth(1);

// 	while (histogram->GetBinContent(lowerBin) - histogram->GetBinContent(upperBin) == 0)
// 	{
// 		lowerBound -= histogram->GetBinWidth(1);
// 		lowerBin = lowerBound / histogram->GetBinWidth(1);
// 		upperBound += histogram->GetBinWidth(1);
// 		upperBin = upperBound / histogram->GetBinWidth(1);
// 	}

// 	double x_1 = lowerBound;
// 	double x_2 = upperBound;
// 	double y_1 = histogram->GetBinContent(lowerBin);
// 	double y_2 = histogram->GetBinContent(upperBin);

// 	// 0 makes the equation unsolveable
// 	if (y_1 == 0)
// 		y_1 = zeroAlt;
// 	if (y_2 == 0)
// 		y_2 = zeroAlt;

// 	startingParameters[1] = (y_1 * x_1 - y_2 * x_2) / y_1 - y_2;
// 	startingParameters[0] = y_1 * (x_1 - startingParameters[1]);
// 	return startingParameters;
// }

std::vector<double> getInitialPowerLawParams(TH1* histogram, double lowerBound, double upperBound)
{
	std::vector<double> startingParameters = {std::pow(10, 17), -400, -7};
	// double x = lowerBound;
	// double y = histogram->GetBinContent(lowerBound / histogram->GetBinWidth(1));	
	// startingParameters[0] = 1000 * (y / (std::pow(x, startingParameters[2])));

	return startingParameters;
}


TFitResultPtr fitToPowerLaw(char const* name, TH1* hist, TFile* file, std::vector<double> params, double lowerBound, double upperBound)
{
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	c1->SetLogy();

	std::cout << "fitting power law to: " << name << "\n";
	auto f1 = new TF1("powerLaw", powerLaw, 0, 3000, 3);
	f1->SetParNames("Scalar", "X Offset", "Power");
	f1->SetParameters(params[0], params[1], params[2]);
	f1->SetNpx(1000);

	TFitResultPtr result = hist->Fit("powerLaw", "SL", "", lowerBound, upperBound);

	double chi2 = __DBL_MAX__;
	while (result->Chi2() < chi2)
	{
		chi2 = result->Chi2();
		f1->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
		result = hist->Fit("powerLaw", "SL", "", lowerBound, upperBound);
	}

	gStyle->SetOptFit(1111);
	file->WriteObject(c1, name);

	return result;
}

TFitResultPtr fitToExponential(char const* name, TH1* hist, TFile* file, std::vector<double> params, double lowerBound, double upperBound)
{
	TCanvas* c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	c1->SetLogy();

	std::cout << "fitting exponential to: " << name << "\n";
	auto f1 = new TF1("exponential", exponential, 0, 3000, 3);

	f1->SetParNames("Scalar", "Base Power", "X Offset");
	f1->SetParameters(params[0], params[1], params[2]);
	f1->SetNpx(1000);

	TFitResultPtr result = hist->Fit("exponential", "SL", "", lowerBound, upperBound);
	double chi2 = __DBL_MAX__;

	while (result->Chi2() < chi2 || result->Prob() < 0.1)
	{
		chi2 = result->Chi2();
		f1->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
		result = hist->Fit("exponential", "SL", "", lowerBound, upperBound);
	}

	gStyle->SetOptFit(1111);
	file->WriteObject(c1, name);

	return result;
}


TFitResultPtr fitToPowerLaw(char const* name, TH1* hist, TFile* file, std::vector<double> params, double lowerBound, double upperBound, int iterations)
{
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	c1->SetLogy();

	std::cout << "fitting power law to: " << name << "\n";
	auto f1 = new TF1("powerLaw", powerLaw, 0, 3000, 3);
	f1->SetParNames("Scalar", "X Offset", "Power");
	f1->SetParameters(params[0], params[1], params[2]);
	f1->SetNpx(1000);

	TFitResultPtr result = hist->Fit("powerLaw", "SL", "", lowerBound, upperBound);

	for (int i = 0; i < iterations; ++i)
	{
		f1->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
		result = hist->Fit("powerLaw", "SL", "", lowerBound, upperBound);
	}

	gStyle->SetOptFit(1111);
	file->WriteObject(c1, name);

	return result;
}

TFitResultPtr fitToExponential(char const* name, TH1* hist, TFile* file, std::vector<double> params, double lowerBound, double upperBound, int iterations)
{
	TCanvas* c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	c1->SetLogy();

	std::cout << "fitting exponential to: " << name << "\n";
	auto f1 = new TF1("exponential", exponential, 0, 3000, 3);

	f1->SetParNames("Scalar", "Base Power", "X Offset");
	f1->SetParameters(params[0], params[1], params[2]);
	f1->SetNpx(1000);

	TFitResultPtr result = hist->Fit("exponential", "SL", "", lowerBound, upperBound);

	for (int i = 0; i < iterations; ++i)
	{
		f1->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
		result = hist->Fit("exponential", "SL", "", lowerBound, upperBound);
	}

	gStyle->SetOptFit(1111);
	file->WriteObject(c1, name);

	return result;
}

TH1* getHist(std::string name, TFile* file)
{
	return dynamic_cast<TH1*>(file->FindObjectAny(name.c_str()));
}

TH1* getPositiveLeptonHist(TH2* histogram)
{
	std::string histName = histogram->GetName();
	histName += " (+) ";
	return histogram->ProjectionX(histName.c_str());
}

TH1* getNegativeLeptonHist(TH2* histogram)
{
	std::string histName = histogram->GetName();
	histName += " (-) ";
	return histogram->ProjectionY(histName.c_str());
}

bool checkToProject(TH1* histogram)
{
	if (dynamic_cast<TH2*>(histogram))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool writeHistogramFit(const TFitResultPtr& fitResults, std::fstream& parameterFile)
{
	std::string fileInput = fitResults->GetName();
	fileInput += " Fit Results:\n";

	for (size_t i = 0; i < fitResults->NPar(); ++i)
	{
		fileInput += fitResults->ParName(i);
		fileInput += ": ";
		fileInput += std::to_string(fitResults->Parameter(i));
		fileInput += " +- ";
		fileInput += std::to_string(fitResults->ParError(i));
		fileInput += "\n";
	}

	parameterFile << fileInput.c_str();

	return true;
}

int getLowestDerivativeBin(TH1* histogram)
{
	double minDerivative = __DBL_MAX__;
	int minBin = 1;
	for (size_t i = 1; i < histogram->GetNbinsX() + 1; ++i)
	{
		double derivative = (histogram->GetBinContent(i + 1) - histogram->GetBinContent(i)) / (histogram->GetBinWidth(1));
		if (derivative < minDerivative)
		{
			minDerivative = derivative;
			minBin = i;
		}
	}

	return minBin * histogram->GetBinWidth(1);
}

std::vector<double> randomizeParameters(const std::vector<double>& parameters)
{
	std::srand(std::time(nullptr));

	std::vector randomParams(parameters);

	for (size_t i = 0; i < randomParams.size(); ++i)
	{
		double randScalar = 10 * (((double)std::rand()) / RAND_MAX);
		randomParams[i] *= randScalar;
	}

	return randomParams;
}