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
#include "TH2.h"
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
class fitInformation {
	public:
		std::vector<double> results;
		std::vector<double> errors;
		std::vector<std::pair<double,double>> ResultError;
		std::string fittedFunction;
		std::string baseFunction;
		int numParams;
		std::string paramName;
}; 

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

std::vector<std::string> generateHistNames();
// Graphs the values of the parameters
void graph(std::map<std::string, std::vector<std::vector<double>>> data, std::string filename);
fitInformation mapMaker9000(TFitResultPtr Result, std::string type, std::string var);

// Fits the graphs of parameters to curves
// must be manually changed 
std::vector<fitInformation> fitParameters(std::string filename, std::string paramGraphsName, int time, std::string Histname);

double ParametrizedDoubleSidedCrystalballFunction(double *x, double *par);
double DoubleSidedCrystalballFunction(double *x, double *par);
double expGaussExp(double *x, double *par);
double breitWigner(double *x, double *par);
double pearson(double *x, double *par);
double studentsT(double *x, double *par);
double hyperbolicSecant(double *x, double *par);
double pearsonIV(double *x, double *par);

TFitResultPtr fitToDSCB(std::string name, TH1* hist, TFile* file, std::vector<double> params,std::string histsname);
TFitResultPtr fitToAlternative(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToBW(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToStudentsT(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToHyperbolicSecant(char const* name, TH1* hist, TFile* file, std::vector<double> params);
TFitResultPtr fitToPearson(char const* name, TH1* hist, TFile* file, std::vector<double> params, int i);
TFitResultPtr fitToPearsonIV(std::string name, TH1* hist, TFile* file, std::vector<double> params, std::string histsname);
void parametrizationGraph(std::vector<fitInformation> fitinformation, std::string saveName, TH1* hist, double mass);
void parametrizationCheck(std::vector<fitInformation> fitinformation, std::vector<char const*> filenames, std::string histname);
std::string getParameters(fitInformation fitinformation);
double globalNorm;
int globalCounter = 0;
int Normalizationerrors;

void newLoopFit()
{
	multipleFits();
	// auto paramData = getParams();
	// graph(paramData);
	//	fitParameters();
}

void multipleFits()
{
	// Filenames of root files that have the graphs to fit in them
	std::vector<char const*> fileNames{"Higgs500.root","Higgs600.root","Higgs700.root","Higgs800.root","Higgs900.root","Higgs1000.root","Higgs1100.root","Higgs1200.root","Higgs1300.root","Higgs1400.root","Higgs1500.root"};
	//std::vector<char const*> fileNames{"Higgs700.root","Higgs800.root","Higgs900.root","Higgs1000.root","Higgs1100.root","Higgs1200.root","Higgs1300.root","Higgs1400.root","Higgs1500.root"};
	// The name of the histogram in the root files above that is bing fitted

	std::vector<std::string> histTypes = generateHistNames();
	//std::vector<std::string> histTypes = {"eee_eeee_Same Sign Invariant Mass;1"};//Testing purposes
	
	// This script creates three root files:
	// The name of the root file the fitted histograms are to be stored
	std::string fitHistsName = "NewpearsonFitHists.root";
	// The name of the root file graphs of parameter values are to be stored
	std::string paramGraphsName = "pearsonParamGraphs.root";
	// The name of the root file the fitted parameter graphs are to be stored
	std::string fitParamsName = "fitParamsNoLow.root";
	// The name of the text file with all parametrizations
	std::string parametrizedFunction = "functionParametrizations.txt";
	TFile* file = TFile::Open(fitHistsName.c_str(), "RECREATE");
	//delete file;
	std::ofstream paramsFile(parametrizedFunction);

	std::string pString = "Histname|||a_low|||a_high|||n_low|||n_high|||mean|||sigma\n";
	paramsFile << pString << '\n';

	int time = 0;	

	TFile* fitParams = TFile::Open(fitParamsName.c_str(), "RECREATE");
	fitParams->Close();

	for (auto Histname : histTypes){
		std::vector<std::vector<double>> results;
		std::vector<std::vector<double>> errors;

		bool doFit = false;
		for (auto filename : fileNames){
			TFile* histFile = TFile::Open(filename);
			//TH1* hist = histFile->Get<TH1>(histType.c_str());
			//std::cout<<Histname.substr(9,5);
			if(Histname.substr(9,5) == "_Reco"){
				int nameLength = Histname.length();
				std::string nHistname = Histname.substr(0,nameLength-1);
				TH2* hist = dynamic_cast<TH2*>(histFile->FindObjectAny(nHistname.c_str()));
				if(hist != nullptr){
					if(Histname.substr(nameLength-1,1) == "X"){
						TH1D* histX = hist->ProjectionX();
						histX->DrawCopy();
							if(histX->GetMean(1) != 0 && histX->GetEntries()>400){
								std::cout<<"Fitting" << Histname <<"\n";
								TFitResultPtr gausResult = histX->Fit("gaus", "SQ", "", 250,2000);
								auto params = gausResult->Parameters();
								std::string rootName(filename);
								int rootPos = rootName.length() - 5;
								std::string name = rootName.substr(0, rootPos) + Histname;
								auto result = fitToDSCB(name, histX, file, params, fitHistsName);
								
								results.push_back(result->Parameters());
								errors.push_back(result->Errors());
								doFit = true;
							}
					}
					else if(Histname.substr(nameLength-1,1) == "Y"){
						TH1D* histY = hist->ProjectionY();
							if(histY->GetMean(1) != 0 && histY->GetEntries()>400){
								TFitResultPtr gausResult = histY->Fit("gaus", "SQ", "", 250,2000);
								auto params = gausResult->Parameters();
								std::string rootName(filename);
								int rootPos = rootName.length() - 5;
								std::string name = rootName.substr(0, rootPos) + Histname;
								//auto result = fitToPearsonIV(name, histY, file, params, fitHistsName);
								//results.push_back(result->Parameters());
								//errors.push_back(result->Errors());
								doFit = false;
							}
					}
					else{
						std::cout<<"Bad Hist Name \n";
					}
				}
			}
			else{
				TH1* hist = getHist(Histname.c_str(), histFile);
				//Check if this is null ptr --------------------------------
				if(hist != nullptr){
					hist->DrawCopy();
					double_t Mean = hist->GetMean(1);
					// Get initial parameters from a gaussian
					if(Mean != 0 && hist->GetEntries()>800){
						doFit = true;
						std::cout<<"Fitting" << Histname << "\n";
						TFitResultPtr gausResult = hist->Fit("gaus", "SQ", "", 250,2000);
						// The params vector is ordered norm, mean, sigma
						auto params = gausResult->Parameters();
						std::string rootName(filename);
						int rootPos = rootName.length() - 5;
						std::string name = rootName.substr(0, rootPos) + Histname;
						//auto result = fitToPearsonIV(name, hist, file, params, fitHistsName);
						auto result = fitToDSCB(name, hist, file, params, fitHistsName);

						results.push_back(result->Parameters());
						errors.push_back(result->Errors());
					}
				}
			}
			histFile->Close();
		}
	//doFit = false;
	if(doFit){
		auto params = paramsFromFits(results, errors);
		graph(params, paramGraphsName); //should generally work still

		auto finalParameters = fitParameters(fitParamsName, paramGraphsName,time, Histname);

		std::string alparam = getParameters(finalParameters.at(0)); //p[0] * 10^-4(x-1)^2 + p[3]
		std::string ahparam = getParameters(finalParameters.at(1));
		std::string nlparam = getParameters(finalParameters.at(2));
		std::string nhparam = getParameters(finalParameters.at(3));
		std::string sigmaparam = getParameters(finalParameters.at(4));	
		
		std::string massparam = getParameters(finalParameters.at(5));	//[0] * 10^-4 + x* [1]	

		//std::string functionString = "("+ nparam +") * std::pow(1 + std::pow((x-("+ massparam +")) / ("+aparam+"),2), -("+mparam+")) * std::exp(-("+nuparam+") * std::atan((x-("+massparam+"))/("+aparam+")))";
		std::string printString = Histname + "|||" + alparam + ahparam  + nlparam + nhparam + sigmaparam + massparam; 
		//std::cout<< printString << "\n";
		paramsFile << printString << '\n';
		
		parametrizationCheck(finalParameters, fileNames, Histname);

	}
	time++;
	std::cout<<Histname << "Done! \n";

}
std::cout<< "number of errors: "<< Normalizationerrors;
}

void parametrizationCheck(std::vector<fitInformation> fitinformation, std::vector<char const*> filenames, std::string histname){
	double mass = 500;
	for(auto filename : filenames){
		TFile* histFile = TFile::Open(filename);
		std::string Sfilename(filename);

		if(histname.substr(9,5) == "_Reco"){
			int nameLength = histname.length();
			std::string nHistname = histname.substr(0,nameLength-1);
			TH2* hist = dynamic_cast<TH2*>(histFile->FindObjectAny(nHistname.c_str()));
			TH1D* histX = hist->ProjectionX();
			std::string saveNameX = "DBSCOverlay_" + Sfilename + histname + "X.png"; 
			parametrizationGraph(fitinformation, saveNameX, histX, mass);
			TH1D* histY = hist->ProjectionY();
			std::string saveNameY = "DBSCOverlay_" + Sfilename + histname + "Y.png"; 
			parametrizationGraph(fitinformation, saveNameY, histY, mass);
		}
		else{
			TH1* hist = getHist(histname.c_str(), histFile);
			std::string saveName = "DBSCOverlay_" + Sfilename + histname + ".png"; 
			parametrizationGraph(fitinformation, saveName, hist,mass);
		}
		//std::cout<<mass;
		mass += 100;
	}
}
void parametrizationGraph(std::vector<fitInformation> fitinformation, std::string saveName, TH1* hist, double mass){
	TCanvas *c1 = new TCanvas("c1", "Superimposed Canvas", 0,0, 1500,500);
	TF1* f1 = new TF1 ("f1", ParametrizedDoubleSidedCrystalballFunction, 0, 2000, 23);
	//al,ah,nl,nh,sigma, mean
	//f1->Setparameters
	std::cout<<"checking: " << saveName << "-----------------------------------------------\n";
	globalNorm = hist->Integral("width");
	//std::cout<< "Check 1: " <<globalNorm << "\n";
	for(int i=0;i<22;i++){
		int number = (i)/4;
		double value;
		if(number < 5){
			value = fitinformation.at(number).results.at(i%4);
			f1->FixParameter(i, value);
		}
		if(number == 5){
			value = fitinformation.at(5).results.at(i%4);
			f1->FixParameter(i, value);
		}
	}
	f1->FixParameter(22, mass);
	f1->SetNpx(1000);
	//hist->GetXaxis()->SetLimits(mass-500,mass+300);
	hist->Draw();

	f1->Draw("SAME");
	//std::cout << "After Integral: " << f1->Integral(0,2000) << "\n";
	c1->Update();
	// Normalization is .3w


	c1->SaveAs(saveName.c_str());
	c1->Close();

}

std::string getParameters(fitInformation fitinformation){
	std::string returnString;
	int i = 0;
	for(auto result: fitinformation.results){
		double rresult;
		if(i == 0){
			rresult = 10000 * result;
		}
		else{
			rresult = result;
		}
		returnString += std::to_string(rresult);
		returnString += "|||";
		i++;
	}
	return returnString;

}

TH1* getHist(std::string name, TFile* file)
    {
    auto hist = dynamic_cast<TH1*>(file->FindObjectAny(name.c_str()));
    
    return hist;
}
std::vector<std::string> generateHistNames() {
	std::vector<std::string> Channels;
	//std::vector<std::string> recoChannels = {"eeee","eeeu","eeuu","eueu","euuu","uuuu","eee","eeu","eue","euu","uue","uuu","ee","e e","eu","e u","uu","u u"};

	std::vector<std::string> recoChannels = {"eeee","eeeu","eeuu","eueu","euuu","uuuu"};
	//std::vector<std::string> recoChannels = {"eeeu"};
	std::vector<std::string> genSimChannels = {"eeee","eeeu", "eeuu", "eueu", "euuu","uuuu"};
	for(auto reco : recoChannels){
		for(auto genSim : genSimChannels){
			if(reco.length() == 4){
				std::string returnString2 = reco + "_" + genSim + "_Reco Invariant Mass Background;1X";
				std::string returnString3 = reco + "_" + genSim + "_Reco Invariant Mass Background;1Y";
				Channels.push_back(returnString2);
				Channels.push_back(returnString3);
			}
			else{
				std::string returnString = reco + "_" + genSim + "_Same Sign Invariant Mass;1";
				Channels.push_back(returnString);
			}
		}
	}
	//save x and y seperately
	return Channels;
}

std::map<std::string, std::vector<std::vector<double>>> paramsFromFits(std::vector<std::vector<double>> results, std::vector<std::vector<double>> errors)
{
	std::map<std::string, std::vector<std::vector<double>>> data;
	std::vector<double> x{500.0,600.0,700.0,800.0,900.0,1000.0,1100.0,1200.0, 1300.0, 1400.0, 1500.0};
	//std::vector<double> x{700.0,800.0,900.0,1000.0,1100.0,1200.0, 1300.0, 1400.0, 1500.0};

	//std::vector<std::string> names{"a", "m", "mass", "nu", "n"};
	std::vector<std::string> names{"alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma"};

	for (int i = 0; i < names.size(); i++)
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
		const int numPoints = 11;//------------------------------------------------------------------------------------------
		const double* x = value.second.at(0).data();
		const double* y = value.second.at(1).data();
		const double* error = value.second.at(2).data();
		// Error for x is zero
		std::vector<double> zeroVector(11, 0.0);
		const double* zero = zeroVector.data();

		auto graph = new TGraphErrors(numPoints, x, y, zero, error);
		graph->SetTitle(value.first.c_str());
		graph->SetMarkerStyle(15);

		std::size_t titleIndex = value.first.find(';');
		std::string name = value.first.substr(0, titleIndex);

		// Uncomment the following two lines for graphs drawn on canvases:
		graph->Draw("AP");
		paramGraphs->WriteObject(graph, name.c_str());
		c1->Close();
	}
	paramGraphs->Close();

 }

std::vector<fitInformation> fitParameters(std::string filename, std::string paramGraphsName, int i, std::string Histname)
{	
	std::string newHistname = Histname.substr(0,Histname.find("_")) + Histname.substr(Histname.find("_")+1, 4);
	TFile* paramGraphs = TFile::Open(paramGraphsName.c_str());
	TFile* fitParams = TFile::Open(filename.c_str(), "update");

	 //"n_{high}", "mean", "sigma"

	std::string mname = newHistname + "mean";
	TCanvas* mcanvas = new TCanvas(mname.c_str(),mname.c_str() , 800, 500);
	TF1* powerLaw = new TF1("powerLaw", "[0]*(x-[1])^[2] + [3]", 0, 2000);

	auto mass = dynamic_cast<TGraph*>(paramGraphs->FindObjectAny("mean"));
	auto fitResultMass = mass->Fit("pol1", "S");
	mass->Draw("AP");
	//fitParams->WriteObject(mcanvas, mname.c_str());	
	fitParams->cd();
	mcanvas->Write();
	auto massResult = mapMaker9000(fitResultMass, "pol1", "mean");


	std::string alname = newHistname + "alpha_{low}";
	TCanvas* alcanvas = new TCanvas(alname.c_str(),alname.c_str() , 800, 500);
	auto al = dynamic_cast<TGraph*>(paramGraphs->FindObjectAny("alpha_{low}"));
	powerLaw->SetParameters(.5, 0, 2, 0);
	//powerLaw->FixParameter(1, 0);
	gStyle->SetOptFit(1111);
	auto fitResultal = al->Fit("powerLaw", "S");
	al->Draw("AP");
	//fitParams->WriteObject(acanvas, aname.c_str());
	fitParams->cd();
	alcanvas->Write();
	powerLaw->ReleaseParameter(1);
	auto alResult = mapMaker9000(fitResultal, "powerLaw",  "alpha_{low}");


	std::string ahname = newHistname + "alpha_{high}";	
	TCanvas* ahcanvas = new TCanvas(ahname.c_str(),ahname.c_str() , 800, 500);
	auto ah = dynamic_cast<TGraph*>(paramGraphs->FindObjectAny("alpha_{high}"));
	powerLaw->SetParameters(.5, 0, 2, 0);
	gStyle->SetOptFit(1111);
	auto fitResultah = ah->Fit("powerLaw", "S");
	ah->Draw("AP");
	//fitParams->WriteObject(m2canvas, m2name.c_str());
	fitParams->cd();
	ahcanvas->Write();
	auto ahResult = mapMaker9000(fitResultah, "powerLaw", "alpha_{high}");



	std::string nlname = newHistname + "n_{low}";
	TCanvas* nlcanvas = new TCanvas(nlname.c_str(),nlname.c_str() , 800, 500);
	auto nl = dynamic_cast<TGraph*>(paramGraphs->FindObjectAny("n_{low}"));
	powerLaw->SetParameters(0.5, 0, 2, .5);
	gStyle->SetOptFit(1111);
	powerLaw->SetMinimum(0.0);
	auto fitResultnl = nl->Fit("powerLaw", "S");
	nl->Draw("AP");
	//fitParams->WriteObject(nucanvas,nuname.c_str());
	fitParams->cd();
	nlcanvas->Write();
	auto nlResult = mapMaker9000(fitResultnl, "powerLaw", "n_{low}");


	std::string nhname = newHistname + "n_{high}";
	TCanvas* nhcanvas = new TCanvas(nhname.c_str(),nhname.c_str() , 800, 500);
	auto nh = dynamic_cast<TGraph*>(paramGraphs->FindObjectAny("n_{high}"));
	powerLaw->SetParameters(0.5, 0, 2, .5);
	powerLaw->SetParLimits(1,170, 1000);
	gStyle->SetOptFit(1111);
	auto fitResultnh = nh->Fit("powerLaw", "S");
	nh->Draw("AP");
	//fitParams->WriteObject(nucanvas,nuname.c_str());
	fitParams->cd();
	nhcanvas->Write();
	auto nhResult = mapMaker9000(fitResultnh, "powerLaw", "n_{high}");



	powerLaw->SetMinimum(-10000000.0);
	powerLaw->SetParLimits(1,-1000,1000);
	std::string sigmaname = newHistname + "sigma";
	TCanvas* sigmacanvas = new TCanvas(sigmaname.c_str(),sigmaname.c_str() , 800, 500);
	auto sigma = dynamic_cast<TGraph*>(paramGraphs->FindObjectAny("sigma"));
	powerLaw->SetParameters(0.5, 0, 2, .5);
	gStyle->SetOptFit(1111);
	auto fitResultsigma = sigma->Fit("powerLaw", "S");
	sigma->Draw("AP");
	//fitParams->WriteObject(nucanvas,nuname.c_str());
	fitParams->cd();
	sigmacanvas->Write();
	auto sigmaResult = mapMaker9000(fitResultsigma, "powerLaw", "sigma");

	// auto n = dynamic_cast<TGraph*>(paramGraphs->FindObjectAny("n"));
	// powerLaw->SetParameters(-107378, 195.667, -.505356, 25964.5);
	// gStyle->SetOptFit(1111);
	// auto fitResultN = n->Fit("powerLaw", "S");
	// n->Draw("AP");
	// std::string nname = Histname + "n";
	// fitParams->WriteObject(c1, nname.c_str());
	// auto nResult = mapMaker9000(fitResultN, false, "n");


	std::vector<fitInformation> returnResult;
	
	returnResult.push_back(alResult);
	returnResult.push_back(ahResult);
	returnResult.push_back(nlResult);
	returnResult.push_back(nhResult);
	returnResult.push_back(sigmaResult);

	returnResult.push_back(massResult);
	//returnResult.push_back(nResult);

	fitParams->Close();

	mcanvas->Close();
	alcanvas->Close();
	ahcanvas->Close();
	nlcanvas->Close();
	nhcanvas->Close();
	sigmacanvas->Close();

	return returnResult;
}

fitInformation mapMaker9000(TFitResultPtr Result, std::string type, std::string var){
	fitInformation rfitInformation;
	auto params = Result->Parameters();
	auto errors = Result->Errors();
	int i = 0;
	std::vector<std::pair<double,double>> paramVector;
	for(auto param : params){
		if(i ==0){
			double newParam = param * 10000;
			std::pair<double,double> resultPair(newParam,errors[i]);
			paramVector.push_back(resultPair);
		}
		else{
		std::pair<double,double> resultPair(param,errors[i]);
		paramVector.push_back(resultPair);

		}
		i++;
	}
	rfitInformation.errors = errors;
	rfitInformation.results = params;
	rfitInformation.ResultError = paramVector;
	if(type == "powerLaw"){
		//x is the true mass
		std::string finalFunction = std::to_string(std::get<0>(paramVector[0])) + "*std::pow(10,-4)* std::pow([0]-" +std::to_string(std::get<0>(paramVector[1])) + ","+std::to_string(std::get<0>(paramVector[2])) +") +" + std::to_string(std::get<0>(paramVector[3]));
		rfitInformation.baseFunction = "[0]*(x-[1])^[2] + [3]";
		rfitInformation.numParams = 4;
		rfitInformation.fittedFunction = finalFunction;
	}
	else if(type == "pol1"){
		std::string finalFunction = std::to_string(std::get<0>(paramVector[0])) + "*std::pow(10,-4) +" + std::to_string(std::get<0>(paramVector[1])) + "*x";
		rfitInformation.baseFunction = "[0]* std::pow(10,-4) +[1]*x";
		rfitInformation.numParams = 2;
		rfitInformation.fittedFunction = finalFunction;

	}	else if(type == "pol2"){
		std::string finalFunction = std::to_string(std::get<0>(paramVector[0])) + "+" + std::to_string(std::get<0>(paramVector[1])) + "*x + std::pow(x,2) * " +std::to_string(std::get<0>(paramVector[3])) ;
		rfitInformation.baseFunction = "[0]+[1]*x + [2]x^2";
		rfitInformation.numParams = 3;
		rfitInformation.fittedFunction = finalFunction;

	}	else if(type == "pol2"){
		std::string finalFunction = std::to_string(std::get<0>(paramVector[0])) + "+" + std::to_string(std::get<0>(paramVector[1])) + "*x + std::pow(x,2) * " +std::to_string(std::get<0>(paramVector[3])) + "+ std::pow(x,3) * " +std::to_string(std::get<0>(paramVector[4]));
		rfitInformation.baseFunction = "[0]+[1]*x + [2]x^2 + [3]x^3";
		rfitInformation.numParams = 4;
		rfitInformation.fittedFunction = finalFunction;
	}
	else{
		rfitInformation.baseFunction = "null";
		rfitInformation.fittedFunction = "null";
	}
	rfitInformation.paramName = var;
	return rfitInformation;
}


double ParametrizedDoubleSidedCrystalballFunction(double *x, double *par)
{
	double alpha_l = par[0]  * std::pow(par[22]-par[1],par[2]) + par[3]; 
	double alpha_h = par[4]  * std::pow(par[22]-par[5],par[6]) + par[7]; 
	double n_l     = par[8]  * std::pow(par[22]-par[9],par[10]) + par[11]; 
	double n_h     = par[12] * std::pow(par[22]-par[13],par[14]) + par[15]; 
	double sigma   = par[16] * std::pow(par[22]-par[17],par[18]) + par[19];
	double mean	= par[20] + par[21]*par[22]; 

	double N = globalNorm;
	float t = (x[0]-mean)/sigma;
	double result;
	double fact1TLessMinosAlphaL = alpha_l/n_l;
	double fact2TLessMinosAlphaL = (n_l/alpha_l) - alpha_l -t;
	double fact1THihgerAlphaH = alpha_h/n_h;
	double fact2THigherAlphaH = (n_h/alpha_h) - alpha_h +t;

	double root2 = std::pow(2,0.5);
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
	
	double lowTailNorm = (n_l/std::abs(alpha_l)) * 1/(n_l - 1) * std::exp(-0.5 * alpha_l * alpha_l);
	double highTailNorm = (n_h/std::abs(alpha_h)) * 1/(n_h - 1) * std::exp(-0.5 * alpha_h * alpha_h);
	double gaussianNormA = erf(std::abs(alpha_l/root2)) + erf(std::abs(alpha_h/root2));  
	double gaussianNormB = std::pow(M_PI/2, 0.5) * gaussianNormA;
	double functionNormalization = std::pow(sigma * (gaussianNormB + lowTailNorm + highTailNorm ), -1);


	globalCounter++;
	if(globalCounter%1000 == 0){
	// std::cout<<"Global norm: " << globalNorm << "\n";
	// std::cout<<"Function Norm: " << functionNormalization << "\n";
	// std::cout<<"n_h: " << n_h << "\n";

	//seems like the Function Normaliztion is being miscalculated
	}
	return N * functionNormalization * result;
}

double DoubleSidedCrystalballFunction(double *x, double *par)
{
	double alpha_l = par[0]; 
	double alpha_h = par[1]; 
	double n_l     = par[2]; 
	double n_h     = par[3]; 
	double mean	= par[4]; 
	double sigma	= par[5];
	double N	= globalNorm;
	float t = (x[0]-mean)/sigma;
	double result;
	double fact1TLessMinosAlphaL = alpha_l/n_l;
	double fact2TLessMinosAlphaL = (n_l/alpha_l) - alpha_l -t;
	double fact1THihgerAlphaH = alpha_h/n_h;
	double fact2THigherAlphaH = (n_h/alpha_h) - alpha_h +t;

	double root2 = std::pow(2,0.5);
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
	
	double lowTailNorm = (n_l/std::abs(alpha_l)) * 1/(n_l - 1) * std::exp(-0.5 * alpha_l * alpha_l);
	double highTailNorm = (n_h/std::abs(alpha_h)) * 1/(n_h - 1) * std::exp(-0.5 * alpha_h * alpha_h);
	double gaussianNormA = erf(std::abs(alpha_l/root2)) + erf(std::abs(alpha_h/root2));  
	double gaussianNormB = std::pow(M_PI/2, 0.5) * gaussianNormA;
	double functionNormalization = std::pow(sigma * (gaussianNormB + lowTailNorm + highTailNorm ), -1);


	globalCounter++;
	if(globalCounter%1000 == 0){
	//std::cout<<"Global norm: " << globalNorm << "\n";
	}
	return N * functionNormalization * result;
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
	//double n = par[4];

	double result;

	//std::complex<double> i(0, 1);
	//double _Complex z = 0 + 1 * _Complex_I;
	gsl_sf_result  resultR;
	gsl_sf_result  resultArg;


	int gammaResult = gsl_sf_lngamma_complex_e(m, (nu/2), &resultR, &resultArg);
	double val = resultR.val;

	double constant =  std::abs(std::exp(resultR.val) / (std::tgamma(m))); 
	double denominator = (a * std::beta(m-0.5,0.5));
	result = globalNorm* constant * constant/denominator * std::pow(1 + std::pow((x[0]-mass) / a,2), -m) * std::exp(-nu * std::atan((x[0]-mass)/a));

	globalCounter++;
	if(globalCounter%1000 == 0){
	//std::cout<< "Automatic Normalization Constant is "<< std::to_string(constant * constant/denominator) << "\n";
	//std::cout<< "Global Normalization Constant is "<< std::to_string(globalNorm) << "\n";
	//std::cout<< "Gamma magnitude is "<< std::to_string(val) << "\n";

	}
	//result = n * std::pow(1 + std::pow((x[0]-mass) / a,2), -m) * std::exp(-nu * std::atan((x[0]-mass)/a));

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

TFitResultPtr fitToDSCB(std::string name, TH1* hist, TFile* file, std::vector<double> params,std::string histsname)
{
	std::cout << "fitting crystal ball\n";
	TF1* f1 = new TF1 ("f1", DoubleSidedCrystalballFunction, 0, 2000, 6);
	f1->SetNpx(1000);
	double mass = params[1];
	// alpha low, alpha high, n low, n high, mean, sigma, norm
	globalNorm = hist->Integral("width");

	f1->SetParameters(2.82606, 2.5, 1.08, 1.136, params[1], params[2]);
	f1->SetParNames("alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma"); 
	f1->SetParLimits(0,0, 10);
	f1->SetParLimits(1,0, 10);
	f1->SetParLimits(2,1,10);
	f1->SetParLimits(3,1,10);
	f1->SetParLimits(4,400,1600);
	if(name.substr(8,8) == "eee_eeee" || name.substr(9,8) == "eee_eeee"){
		std::cout<<"EXCEPTION EXCEPTED\n";
		if(name.substr(5,3) == "500"){
			f1->SetParameters(1.624,1.439,1.288,3.151,498.9,5.451);
		} 
		else if(name.substr(6,3) == "1300" || name.substr(6,3) == "1500" ){
			f1->SetParLimits(6,3,25);
			std::cout<<"exception found";
		}
		else{
			f1->SetParLimits(6,3,20);
		}
	}
	


	f1->SetRange(0, 2000);
	f1->SetLineColor(kRed);
	TCanvas *c1 = new TCanvas("c1","Fit Canvas",0,0,1500,500);
	TFitResultPtr results = hist->Fit(f1, "SE");
	gStyle->SetOptFit(111111);
	//file->WriteObject(c1, name);
	std::string Graphname = name + "DBSCball"+ ".png";
	TPaveStats *st = (TPaveStats*)hist->FindObject("stats");
	st->SetX1NDC(0.1);
	st->SetX2NDC(0.5);
	//hist->GetXaxis()->SetRange(mass-450, mass + 350);
	c1->SaveAs(Graphname.c_str());
	c1->Close();

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
	f1->SetParameters(.5,  params[0], params[1]);
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
	//auto f1 = new TF1("pearsonIV", pearsonIV, 0, 2000, 5);
	auto f1 = new TF1("pearsonIV", pearsonIV, 0, 2000, 4);

	globalNorm = hist->Integral("width");

	//f1->SetParNames("a", "m", "mass", "nu", "n");
	f1->SetParNames("a", "m", "mass", "nu");

	f1->SetRange(params[1]-params[2], params[1]+params[2]);

	//f1->SetParameters(50, 5, params[1], 5, params[0]);
	f1->SetParameters(50, 5, params[1], 0.5);

	//f1->Draw();
	// f1->SetNpx(1000);
	f1->SetParLimits(0, 0, 1000);
	f1->SetParLimits(1, 0.5, 10000);
	f1->SetParLimits(2, 250, 2000);
	f1->SetParLimits(3, -2, 4);

	//f1->SetParLimits(4, 10, 10000);

	TFitResultPtr result = hist->Fit("pearsonIV", "S");
	gStyle->SetOptFit(1111);
	//writes to the file
	std::string Graphname = name + ".png";
	TPaveStats *st = (TPaveStats*)hist->FindObject("stats");
	st->SetX1NDC(0.1);
	st->SetX2NDC(0.5);
	//file->WriteObject(c1, name.c_str());
	//f1->Draw("C");
	c1->SaveAs(Graphname.c_str());
	//c1->SaveAll(file);
	c1->Close();

	return result;
}
