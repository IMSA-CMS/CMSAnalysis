#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>
#include <map>
#include <algorithm>
#include <complex>
#include "TH1.h"
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
#include "TROOT.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include <gsl/gsl_sf_result.h>
#include <gsl/gsl_sf_gamma.h>

// Fits multiple graphs with the same function
void multipleFits();

TH1* getHist(string name, TFile* file);

// Gets params from the fits and arranges them into a map
std::map<std::string, std::vector<std::vector<double>>> paramsFromFits(std::vector<std::vector<double>> results, 
	std::vector<std::vector<double>> errors);

// Old function--if you happen to have your params in a text file, this will arrange them into a map
std::map<std::string, std::vector<std::vector<double>>> getParams();

// A function that formats titles into the silly way root wants them
std::string joinTitles(std::vector<std::string> list);

std::vector<std::string> tokenize(std::string line);
// Graphs the values of the parameters
void graph(std::map<std::string, std::vector<std::vector<double>>> data, std::string filename);

// Fits the graphs of parameters to curves
// must be manually changed 
void fitParameters(std::string filename, std::string paramGraphsName);

double DoubleSidedCrystalballFunction(double *x, double *par);
double expGaussExp(double *x, double *par);
double breitWigner(double *x, double *par);
double pearson(double *x, double *par);
double studentsT(double *x, double *par);
double hyperbolicSecant(double *x, double *par);
double pearsonIV(double *x, double *par);

TFitResultPtr fitToDSCB(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToAlternative(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToBW(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToStudentsT(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToHyperbolicSecant(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToPearson(char const* name, TH1* hist, TFile* file, std::vector<double> params, int i);
TFitResultPtr fitToPearsonIV(std::string name, TH1* hist, TFile* file, std::vector<double> params, std::string histsname);

void loopFit()
{
	multipleFits();
	// auto paramData = getParams();
	// graph(paramData);
	//	fitParameters();
}

void multipleFits()
{
	std::vector<std::vector<double>> results;
	std::vector<std::vector<double>> errors;

	// Filenames of root files that have the graphs to fit in them
	//~vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/Higgs1400.root 
	
	std::vector<char const*> fileNames{"test.root"};
	//std::vector<char const*> fileNames{"~vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/Higgs1400.root"};
	// The name of the histogram in the root files above that is bing fitted
	//std::string histType = "MuonMuon Reco Same Sign Invariant Mass;1";
	std::vector<std::string> histTypes = {"eeuu_Reco Same Sign Invariant Mass;1"};
	std::string histType = "eeuu_Reco Same Sign Invariant Mass;1";
	//std::string histType = "eeuu_Muon Same Sign Invariant Mass;1";
	
	// This script creates three root files:
	// The name of the root file the fitted histograms are to be stored
	std::string fitHistsName = "NewpearsonFitHists.root";
	// The name of the root file graphs of parameter values are to be stored
	std::string paramGraphsName = "pearsonParamGraphs.root";
	// The name of the root file the fitted parameter graphs are to be stored
	std::string fitParamsName = "fitParams.root";

	TFile* file = TFile::Open(fitHistsName.c_str(), "RECREATE");
	//delete file;


	int Masscounter = 5;
	for (auto Histname : histTypes){
	for (auto filename : fileNames)
	{
		TFile* histFile = TFile::Open(filename);
		//TH1* hist = histFile->Get<TH1>(histType.c_str());
		TH1* hist = getHist(Histname.c_str(), histFile);

		hist->DrawCopy();
		hist->SetStats(0);
		// Get initial parameters from a gaussian
		TFitResultPtr gausResult = hist->Fit("gaus", "SQ", "", 1000,2000);
		// The params vector is ordered norm, mean, sigma
		auto params = gausResult->Parameters();


		std::string rootName(filename);
		int rootPos = rootName.length() - 5;
		std::string name = rootName.substr(0, rootPos) + Histname;
		
		//int mass = counter*200 + 300;
		int mass = 1400;
		
		//auto result = fitToHyperbolicSecant(name.c_str(), hist, file, params);
		auto result = fitToPearsonIV(name, hist, file, params, fitHistsName);
		//auto result = fitToPearson(name.c_str(), hist, file, params,counter);
		//auto result = fitToStudentsT(name.c_str(), hist, file, params);
		//auto result = fitToBW(name.c_str(), hist, file, params);
		//auto result = fitToDSCB(name.c_str(), hist, file, params);
		//auto result = fitToAlternative(name.c_str(), hist, file, params);
		
		
		results.push_back(result->Parameters());
		errors.push_back(result->Errors());
		Masscounter++;
	}


	auto params = paramsFromFits(results, errors);
	//graph(params, paramGraphsName);

	// uncomment the below line if ready to fit the values of the parameters (the code for fitting parameters is found in fitParameters())
	// fitParameters(fitParamsName, paramGraphsName);
}
}

TH1* getHist(std::string name, TFile* file)
    {
    auto hist = dynamic_cast<TH1*>(file->FindObjectAny(name.c_str()));
    if (!hist)
        throw std::runtime_error(name + " not found!");
    return hist;
}
std::map<std::string, std::vector<std::vector<double>>> paramsFromFits(std::vector<std::vector<double>> results, std::vector<std::vector<double>> errors)
{
	std::map<std::string, std::vector<std::vector<double>>> data;
	std::vector<double> x{1400.0};
	std::vector<std::string> names{"a", "m", "mu", "nu", "n"};

	for (int i = 0; i < 5; i++)
	{
		std::vector<double> y;
		std::vector<double> e;
		for (const auto& vec : results)
		{
			y.push_back(vec.at(i));
		}
		for (const auto& error : errors)
		{
			e.push_back(error.at(i));

		}
		std::string name = joinTitles({names.at(i), "mass", names.at(i), "error"});
		data.insert({name, {x, y, e}});
	}	
	return data;
}

std::map<std::string, std::vector<std::vector<double>>> getParams()
{
	int counter = 0;
	std::string filename = "fitData.txt";
	std::ifstream paramFile(filename);
	std::string line;
	
	if (!paramFile)
	{
		throw std::runtime_error("Cannot access" + filename);
	}

	std::map<std::string, std::vector<std::vector<double>>> graphData;
	// Params vector is ordered x, y, error
	std::vector<double> x, y, e;
	std::vector<std::vector<double>> params = {x, y, e}; 
	// Titles vector is ordered graphTitle, xtitle, ytitle, error
	std::vector<std::string> titles;
	bool axisTitles = false;
	while (std::getline(paramFile, line))
	{
		if (line.empty()) 
		{
			continue; 
		}

		std::vector<std::string> values = tokenize(line);
		if (values.size() == 1)
		{
			if (!titles.empty())
			{
				std::string titleString = joinTitles(titles);
				graphData.insert({titleString, params});

				// clear everything
				titles.clear();
				for (auto& vec : params)
				{
					vec.clear();
				}
			}
			// start new graph!
			titles.push_back(values[0]);
			counter = 0;
			continue;
		}

		
		for (auto& value : values)
		{
			try
			{
				double newValue = std::stod(value);
				params[counter].push_back(newValue);
			}
			catch(const std::invalid_argument& e)
			{
				titles.push_back(value);
			}
		}
		counter++;
	}
	std::string titleString = joinTitles(titles);
	graphData.insert({titleString, params});
	paramFile.close();

	return graphData;
}

std::vector<std::string> tokenize(std::string line)
{
	std::istringstream iss(line);
	std::string token;
	std::vector<std::string> tokens;
	while(getline(iss, token, '\t'))
	{
		tokens.push_back(token);
	}
	return tokens;
}

std::string joinTitles(std::vector<std::string> list)
{
	std::ostringstream oss;
	for (auto& title : list)
	{
		oss << title << ';';
	}
	std::string titleString = oss.str();
	titleString.pop_back();
	return titleString;
}

void graph(std::map<std::string, std::vector<std::vector<double>>> data, std::string filename)
 {
	TFile* paramGraphs = TFile::Open(filename.c_str(), "RECREATE");
	
	// int i = 0;
	for (auto const& value : data)
	{
		TCanvas* c1 = new TCanvas("c1", "Graph Canvas", 800, 500);
		const int numPoints = 6;
		const double* x = value.second.at(0).data();
		const double* y = value.second.at(1).data();
		const double* error = value.second.at(2).data();
		// Error for x is zero
		std::vector<double> zeroVector(6, 0.0);
		const double* zero = zeroVector.data();

		auto graph = new TGraphErrors(numPoints, x, y, zero, error);
		graph->SetTitle(value.first.c_str());
		graph->SetMarkerStyle(15);

		std::size_t titleIndex = value.first.find(';');
		std::string name = value.first.substr(0, titleIndex);

		// Uncomment the following two lines for graphs drawn on canvases:
		graph->Draw("AP");
		paramGraphs->WriteObject(c1, name.c_str());
		paramGraphs->WriteObject(graph, name.c_str());
	
	}
 }

void fitParameters(std::string filename, std::string paramGraphsName)
{	
	TFile* fitParams = TFile::Open(filename.c_str(), "RECREATE");
	TFile* paramGraphs = TFile::Open(paramGraphsName.c_str());
	TCanvas* c1 = new TCanvas("c1", "Fit Canvas", 800, 500);
	TF1* powerLaw = new TF1("powerLaw", "[0]*(x-[1])^[2] + [3]", 0, 800);

	TGraph* mass = paramGraphs->Get<TGraph>("mu;1");
	mass->Fit("pol1");
	mass->Draw("AP");
	fitParams->WriteObject(c1, "mu");	

	TGraph* a = paramGraphs->Get<TGraph>("a;1");
	powerLaw->SetParameters(.5, 0, 2, 0);
	powerLaw->FixParameter(1, 0);
	gStyle->SetOptFit(1111);
	a->Fit("powerLaw");
	a->Draw("AP");
	fitParams->WriteObject(c1, "a");
	powerLaw->ReleaseParameter(1);
	
	TGraph* m = paramGraphs->Get<TGraphErrors>("m;1");
	powerLaw->SetParameters(.5, 0, 2, 0);
	gStyle->SetOptFit(1111);
	m->Fit("powerLaw");
	m->Draw("AP");
	fitParams->WriteObject(c1, "m");

	TGraph* n = paramGraphs->Get<TGraph>("n;1");
	powerLaw->SetParameters(-107378, 195.667, -.505356, 25964.5);
	gStyle->SetOptFit(1111);
	n->Fit("powerLaw");
	n->Draw("AP");
	fitParams->WriteObject(c1, "n");
}

double DoubleSidedCrystalballFunction(double *x, double *par)
{
	double alpha_l = par[0]; 
	double alpha_h = par[1]; 
	double n_l     = par[2]; 
	double n_h     = par[3]; 
	double mean	= par[4]; 
	double sigma	= par[5];
	double N	= par[6];
	float t = (x[0]-mean)/sigma;
	double result;
	double fact1TLessMinosAlphaL = alpha_l/n_l;
	double fact2TLessMinosAlphaL = (n_l/alpha_l) - alpha_l -t;
	double fact1THihgerAlphaH = alpha_h/n_h;
	double fact2THigherAlphaH = (n_h/alpha_h) - alpha_h +t;

	if (-alpha_l <= t && alpha_h >= t)
	{
		result = exp(-0.5*t*t);
	}
	else if (t < -alpha_l)
	{
		result = exp(-0.5*alpha_l*alpha_l)*pow(fact1TLessMinosAlphaL*fact2TLessMinosAlphaL, -n_l);
	}
	else if (t > alpha_h)
	{
		result = exp(-0.5*alpha_h*alpha_h)*pow(fact1THihgerAlphaH*fact2THigherAlphaH, -n_h);
	}
	return N*result;
}
double expGaussExp(double *x, double *par)
{
	double k_l = par[0]; 
	double k_h = par[1]; 
	double mean	= par[2]; 
	double sigma	= par[3];
	double N	= par[4];
	float t = (x[0]-mean)/sigma;
	double result;

	if (-k_l < t && k_h >= t)
	{
		result = exp(-0.5*t*t);
	}
	else if (t <= -k_l)
	{
		result = exp((0.5 * k_l * k_l) + (k_l * t));
	}
	else if (t > k_h)
	{
		result = exp((0.5 * k_h * k_h) - (k_h * t));
	}
	return N*result;
}
double breitWigner(double *x, double *par)
{
	//Parameters are ordered m, gamma 
	double m = par[0];
	double gamma = par[1];

	double g = std::sqrt((m * m) * ((m * m) + (gamma * gamma)));
	double k = (2 * std::sqrt(2) * m * gamma * g) / (M_PI * std::sqrt((m * m) + g));
	double result;

	result = k / ((x[0] * x[0] - m*m) * (x[0] * x[0] - m*m) + m*m * gamma*gamma);
	return result;
}
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
double pearsonIV(double *x, double *par)
{
	double a = par[0];
	double m = par[1];
	double mass = par[2];
	double nu = par[3];
	double n = par[4];
	double result;

	//std::complex<double> i(0, 1);
	//double _Complex z = 0 + 1 * _Complex_I;
	gsl_sf_result  resultR;
	gsl_sf_result  resultArg;


	int gammaResult = gsl_sf_lngamma_complex_e(m, (nu/2), &resultR, &resultArg);
	double val = resultR.val;

	double constant = n * std::abs(std::pow(10, resultR.val) / (std::tgamma(m)))/(a * std::beta(m-0.5,0.5));
	result = constant * std::pow(1 + std::pow((x[0]-mass) / a,2), -m) * std::exp(-nu * std::atan((x[0]-mass)/a));

	return result;
}
double studentsT(double *x, double *par)
{
	double nu = par[0];
	double c = par[1];
	double m = par[2];
	double result;

	double a = (nu + 1)/2;
	double b = (1 + ((x[0]-m) * (x[0]-m)) / nu);
	double d = std::tgamma(a) / (std::sqrt(nu * M_PI) * (nu / 2));
	
	result = c * d * std::pow(b, -a);
	return result;
}
double hyperbolicSecant(double *x, double *par)
{
	double mass = par[0];
	double n = par[1];
	double result = 0.5 * (1/std::cosh((M_PI/2) * (x[0] - mass)));
	return n * result;
}

TFitResultPtr fitToDSCB(char const* name, TH1* hist, TFile* file, std::vector<double> params)
{
	std::cout << "fitting crystal ball\n";
	TF1* f1 = new TF1 ("f1", DoubleSidedCrystalballFunction, 0, 2000, 7);
	f1->SetNpx(500);
	double mass = params[1];
	// alpha low, alpha high, n low, n high, mean, sigma, norm
	f1->SetParameters(2.82606, 5.442, 1.08, 1.136, params[1], params[2], params[0]);
	f1->SetParNames ("alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma", "Norm"); 
	// f1->FixParameter(0, 2.7213);
	// f1->FixParameter(1, 3.21973);
	// f1->FixParameter(2, 1.08);
	// f1->FixParameter(3, 1.13612);
	f1->SetRange(0, 2000);
	f1->SetLineColor(kRed);
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	TFitResultPtr results = hist->Fit(f1, "SRL");
	gStyle->SetOptFit(111111);
	file->WriteObject(c1, name);
	return results;
}
TFitResultPtr fitToAlternative(char const* name, TH1* hist, TFile* file, std::vector<double> params)
{
	
	std::cout << "Fitting ExpGaussExp\n";
	TF1* f1 = new TF1("f1", expGaussExp, 0, 1500, 5);
	// k low, k high, mean, sigma, norm
	f1->SetParameters(3, 3, params[1], params[2], params[0]);
	f1->SetParNames("k_{low}","k_{high}", "mean", "sigma", "Norm");
	f1->SetLineColor(kRed);
	f1->SetNpx(500);
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	TFitResultPtr results = hist->Fit(f1, "L");
	gStyle->SetOptFit(111111);
	file->WriteObject(c1, name);
	return results;
}
TFitResultPtr fitToBW(char const* name, TH1* hist, TFile* file, std::vector<double> params)
{
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	std::cout << "Fitting Breit-Wigner to: " << name << "\n";
	auto f1 = new TF1("breit_wigner", breitWigner, 0, 2000, 2);
	f1->SetParNames("mass", "width");
	// f1->SetRange(params[1] - params[2]*2, params[1] + params[2]*2);
	f1->SetParameters(params[1], 0.6371/params[0]);
	f1->Draw();
	TFitResultPtr results = hist->Fit("breit_wigner");
	file->WriteObject(c1, name);
	gStyle->SetOptFit(111111);
	return results;
}
TFitResultPtr fitToStudentsT(char const* name, TH1* hist, TFile* file, std::vector<double> params)
{

	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	std::cout << "Fitting Students T to: " << name << "\n";
	auto f1 = new TF1("studentsT", studentsT, 0, 2000, 3);
	f1->SetParNames("nu", "n", "mass");
	f1->SetRange(params[1] - 200, params[1] + 200);
	f1->SetParameters(.5, params[0], params[1]);
	// f1->Draw();
	f1->SetNpx(1000);
	auto results = hist->Fit("studentsT", "L");
	gStyle->SetOptFit(111111);
	file->WriteObject(c1, name);
	return results;
}
TFitResultPtr fitToHyperbolicSecant(char const* name, TH1* hist, TFile* file, std::vector<double> params)
{
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	std::cout << "fitting hyperbolic secant to: " << name << "\n";
	auto f1 = new TF1("hyperbolic_secant", hyperbolicSecant, 0, 2000, 2);
	f1->SetParNames("n", "mass");
	// f1->SetRange(params[1] - params[2]*2, params[1] + params[2]*2);
	f1->SetParameters(2*params[0], params[1]);
	// f1->Draw();
	f1->SetNpx(1000);
	TFitResultPtr result = hist->Fit("hyperbolic_secant", "L");
	file->WriteObject(c1, name);
	gStyle->SetOptFit(1111);
	return result;
}
TFitResultPtr fitToPearson(char const* name, TH1* hist, TFile* file, std::vector<double> params, int i)
{
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	std::cout << "fitting pearson to: " << name << "\n";
	auto f1 = new TF1("pearson", pearson, 0, 2000, 4);
	f1->SetParNames("a", "m", "mass", "n");
	f1->SetRange(params[1] - ((6.5-0.5*i)*params[2]), params[1] + ((6.5-0.5*i)*params[2]));
	f1->SetParameters(55, 3.5, params[1], 22000);
	// f1->Draw();
	// f1->SetNpx(1000);
	TFitResultPtr result = hist->Fit("pearson", "RSL");
	gStyle->SetOptFit(1111);
	file->WriteObject(c1, name);
	return result;
}

TFitResultPtr fitToPearsonIV(std::string name, TH1* hist, TFile* file, std::vector<double> params, std::string histsname)
{

	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	std::cout << "fitting pearson to: " << name << "\n";
	auto f1 = new TF1("pearsonIV", pearsonIV, 0, 2000, 5);
	f1->SetParNames("a", "m", "mass", "nu", "n");
	f1->SetRange(1200, 2000);
	//1200,1600
	f1->S
	f1->SetParameters(50, 5, 1400, 5, 62000);

	//f1->Draw();
	// f1->SetNpx(1000);

	TFitResultPtr result = hist->Fit("pearsonIV", "S");
	gStyle->SetOptFit(1111);
	//writes to the file
	std::string Graphname = name + ".png";
	//file->WriteObject(c1, name.c_str());
	TFile* Savefile = TFile::Open(histsname.c_str());
	c1->Write(name.c_str());
	hist->Write("test2");

	

	c1->SaveAs(Graphname.c_str());
	//c1->SaveAll(file);
	return result;
}