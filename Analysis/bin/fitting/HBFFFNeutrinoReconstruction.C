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



// run in batch mode for faster processing: root -b HiggsBackgroundFit.C+
void HBFFFNeutrinoReconstruction()
{
	std::cout << "[INFO] Starting HiggsBackgroundFitFromFile()" << std::endl;
	//const double min = 200;
	//const double max = 2000;
	
	//Output in OtherBackgroundFits
	TFile* outputFile = TFile::Open("/uscms/home/bhobbs/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Output/noNeutrinoZZ.root");
	if (!outputFile || outputFile->IsZombie())
	{
		std::cerr << "Error creating output file!" << std::endl;
		return;
	}
	auto dir = outputFile->Get<TDirectory>("hists");
	dir = dir->Get<TDirectory>("ZVeto");
	dir = dir->Get<TDirectory>("Reco eeee");
	if (!dir)
	{	
		std::cerr << "Error creating directory in output file!" << std::endl;
		return;
	}
	TH1* hist = dir->Get<TH1>("Reco Invariant Mass Background X Projection");
	if (!hist)
	{
		std::cerr << "Error retrieving histogram from output file!" << std::endl;
		return;
	}
	Fitter fitter("","","","");
	FitFunction func = FitFunction::createFunctionOfType(FitFunction::FunctionType::PowerLaw, "Background", "", 0, 2000, "Background");
	std::vector<FitFunction> functions = {func};
	FitFunctionCollection coll(functions);
	fitter.loadFunctions(coll);
	std::unordered_map<std::string, TH1*> histogramMap = {{"Background", hist}};
	fitter.fitFunctions(histogramMap);
	return;
	// std::vector<std::string> backgrounds = {
	// 	// "t#bar{t} Background 1",
	// 	// "t#bar{t} Background 2",
	// 	// "t#bar{t} Background 3",
	// 	"Drell-Yan Background",
	// 	// "Drell-Yan Background No Veto",
	// 	"QCD Background",
	// 	"ZZ Background",
	// 	// "ZZ Background No Veto",
	// 	// "ZZZ",
	// 	// "WW",
	// 	// "WWW",
	// 	// "WWZ",
	// 	// "WZ",
	// 	// "WZZ",
	// 	"t#bar{t}, Multiboson Background",
	// };
}