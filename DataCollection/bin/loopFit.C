#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFormula.h"
#include "TFitResult.h"
#include "TGraphErrors.h"

// double sided crystal ball taken from internet 
double DoubleSidedCrystalballFunction(double *x, double *par);
double expGaussExp(double *x, double *par);

std::map<std::string, std::vector<std::vector<double>>> getParams();
void paramsFromFits(std::vector<std::vector<double>> results, std::vector<std::vector<double>> errors);
void multipleFits();
std::vector<TGraph*> graph(std::map<std::string, std::vector<std::vector<double>>> data);
// A function that formats titles into the silly way root wants them
std::string joinTitles(std::vector<std::string> list);
std::vector<std::string> tokenize(std::string line);
void fitParameters();
void fitNorm();

TFitResultPtr fitToDSCB(std::vector<double> params, int x, TFile* file, TH1* hist, char const* name);
void fitToAlternative(std::vector<double> params, TFile* file, TH1* hist, char const* name);


void loopFit()
{
	
	// multipleFits();
	// auto paramData = getParams();
	// graph(paramData);
	// fitParameters();
}

void multipleFits()
{
	std::vector<std::vector<double>> results;
	std::vector<std::vector<double>> errors;
	std::vector<char const*> fileNames{"HiggsPick300_HiggsBackground.root", "HiggsPick500_HiggsBackground.root", 
	"HiggsPick700_HiggsBackground.root", "HiggsPick900_HiggsBackground.root", "HiggsPick1100_HiggsBackground.root",
	"HiggsPick1300_HiggsBackground.root"};
	TFile* fitHists = TFile::Open("fixedFitHists.root", "RECREATE");

	int counter = 0;

	for (auto filename : fileNames)
	{
		std::string rootName(filename);
		int rootPos = rootName.length() - 5;
		std::string name = rootName.substr(0, rootPos);
		// figuring out parameters
		std::cout << "fitting gaussian to " << filename << "\n";
		TFile* file = TFile::Open(filename);
		TH1* hist = file->Get<TH1>("MuonMuon Reco Same Sign Invariant Mass;1");

		TFitResultPtr gausResult = hist->Fit("gaus", "SQ");
		// The params vector is ordered norm, mean, sigma, alpha low, alpha high, n low, n high
		auto params = gausResult->Parameters();

		auto result = fitToDSCB(params, counter, fitHists, hist, name.c_str());
		results.push_back(result->Parameters());
		errors.push_back(result->Errors());
		// fitToAlternative(params, fitHists, hist, name.c_str());
	
		counter++;
	}
	paramsFromFits(results, errors);
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
		// std::cout << "Reading a line of the textfile\n";
		if (line.empty()) 
		{
			continue; 
		}

		std::vector<std::string> values = tokenize(line);
		if (values.size() == 1)
		{
			// std::cout << "found a title\n";
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
			// std::cout << "Processing value " << value;
			try
			{
				double newValue = std::stod(value);
				params[counter].push_back(newValue);
				// std::cout << " pushed back to vector number " << counter << '\n';
				// std::cout << "It has " << params[counter].size() << " values\n" ;
			}
			catch(const std::invalid_argument& e)
			{
				// std::cout << ", It is a title\n";
				titles.push_back(value);
			}
		}
		// std::cout << "Finished processing values\n";
		counter++;
	}
	std::string titleString = joinTitles(titles);
	graphData.insert({titleString, params});
	paramFile.close();

	return graphData;
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
		result = exp(-0.5*alpha_h*alpha_h)*pow(fact1THihgerAlphaH*fact1THihgerAlphaH, -n_h);
	}
	return N*result;
}

 std::vector<TGraph*> graph(std::map<std::string, std::vector<std::vector<double>>> data)
 {
	// TFile* paramGraphs = TFile::Open("fixedParamGraphs.root", "RECREATE");
	TFile* fittedParams = TFile::Open("fittedParams.root", "RECREATE");
	std::vector<TGraph*> graphsForFitting;
	
	int i = 0;
	for (auto const& value : data)
	{
		TCanvas* c1 = new TCanvas("c1", "Graph Canvas", 800, 500);
		const int numPoints = 6;
		const double* x = value.second.at(0).data();
		const double* y = value.second.at(1).data();
		const double* error = value.second.at(2).data();
		// Error for x is zero
		vector<double> zeroVector(6, 0.0);
		const double* zero = zeroVector.data();

		auto graph = new TGraphErrors(numPoints, x, y, zero, error);
		graph->SetTitle(value.first.c_str());
		graph->SetMarkerStyle(21);

		std::size_t titleIndex = value.first.find(';');
		std::string name = value.first.substr(0, titleIndex);
		
		if (i == 2)
		{
			graph->Fit("pol1");
			graph->Draw("AP");
			fittedParams->WriteObject(c1, name.c_str());
		}
		else if (i == 3)
		{
			TF1* f2 = new TF1("exponential", "[0]^x + [1]", 0, 800);
			f2->SetParameters(-2, 190);
			f2->SetParNames("base", "intercept");
			graph->Fit("exponential");
			graph->Draw("AP");
			fittedParams->WriteObject(c1, name.c_str());
		}
		else if (i == 4)
		{
			TF1* f1 = new TF1("power_law", "[0] * x^[1] + [2]", 0, 800);
			f1->SetParameters(1, 1.2, 10);
			f1->SetParNames("constant", "power", "intercept");
			graph->Fit("power_law");
			graph->Draw("AP");
			fittedParams->WriteObject(c1, name.c_str());
		}

		

		// paramGraphs->WriteObject(c1, name.c_str());
		
		

		i++;
	}
	// paramGraphs->Close();
	return graphsForFitting;
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

void fitParameters(std::vector<TGraph*> graphs)
{
	std::cout << "Inside the function\n"; 
	TFile* fitParams = TFile::Open("fitParams.root", "RECREATE");
	TFile* paramGraphs = TFile::Open("fixedParamGraphs.root");
	TCanvas* c1 = new TCanvas("c1", "Fit Canvas", 800, 500);

	// Graphs at 0 should be norm

	// graphs.at(0)->Fit("expo");

	TGraph* norm = paramGraphs->Get<TGraph>("Norm;1");
	std::cout << "here 1\n";
	norm->Draw("AP");
	std::cout << "here 2\n";
	// norm->Fit("expo");
	fitParams->WriteObject(c1, "Norm");	

	// Graphs at 1 should be mu
	// graphs.at(1)->Fit("pol1");
	// fitParams->WriteObject(c1, "Mu");

	// Graphs at 2 should be sigma
	// TF1* f1 = new TF1("power_law", "[0] * x^[1] + [2]", 0, 800);
	// f1->SetParameters(1, 1.2, 10);
	// f1->SetParNames("constant", "power", "intercept");
	// graphs.at(2)->Fit("f1");
	// fitParams->WriteObject(c1, "sigma");
	
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

TFitResultPtr fitToDSCB(std::vector<double> params, int x, TFile* file, TH1* hist, char const* name)
{
	std::cout << "fitting crystal ball\n";
	TF1* f1 = new TF1 ("f1", DoubleSidedCrystalballFunction, 0, 1500, 7);
	f1->SetNpx(500);

	// alpha low, alpha high, n low, n high, mean, sigma, norm
	f1->SetParameters(2.82606, 5.442, 1.08, 1.136, params[1], params[2], params[0]);
	f1->SetParNames ("alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma", "Norm"); 
	f1->FixParameter(0, 2.7213);
	f1->FixParameter(1, 3.21973);
	f1->FixParameter(2, 1.08);
	f1->FixParameter(3, 1.13612);
	f1->SetLineColor(kRed);
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	TFitResultPtr results = hist->Fit(f1, "SL");
	gStyle->SetOptFit(111111);
	file->WriteObject(c1, name);
	return results;
}

void fitToAlternative(std::vector<double> params, TFile* file, TH1* hist, char const* name)
{
	std::cout << "Fitting ExpGaussExp\n";
	TF1* f1 = new TF1("f1", expGaussExp, 0, 1500, 5);
	
	// k low, k high, mean, sigma, norm
	f1->SetParameters(3, 3, params[1], params[2], params[0]);
	f1->SetParNames("k_{low}","k_{high}", "mean", "sigma", "Norm");
	f1->SetLineColor(kRed);
	f1->SetNpx(500);
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	hist->Fit(f1, "L");
	gStyle->SetOptFit(111111);
	file->WriteObject(c1, name);
}

void paramsFromFits(std::vector<std::vector<double>> results, std::vector<std::vector<double>> errors)
{
	std::map<std::string, std::vector<std::vector<double>>> data;
	std::vector<double> x{300.0, 500.0, 700.0, 900.0, 1100.0, 1300.0};
	std::vector<std::string> names{"Alpha_{low}", "Alpha_{high}", "n_{low}", "n_{high}", "Mu", "Standard Deviation", "Norm"};

	for (int i = 0; i < 7; i++)
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
	auto graphs = graph(data);
	// fitParameters(graphs);
}