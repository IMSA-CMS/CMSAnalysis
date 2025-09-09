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

std::string path = "/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_peter/";
TH1* combineHists (std::vector<std::string> fileNames, std::string channel, std::string histName);
std::vector<std::string> years = {"2016", "2017", "2018"};
std::vector<std::string> channelTypes =
{
	"eeee", "eeem", "emem", "eemm", "emmm", "mmmm"
	//"ee",
	//"eu",
	//"uu",
};

std::vector<std::string> histogramTypes = 
{
	// "X Projection",
	// "Y Projection",
	// "Same Sign Invariant Mass",
	"h_mll1",
	"h_mll2",
};

// run in batch mode for faster processing: root -b HiggsBackgroundFit.C+
void HiggsBackgroundFitFromFile()
{
	std::cout << "[INFO] Starting HiggsBackgroundFitFromFile()" << std::endl;
	// const double min = 200;
	// const double max = 2000;

	std::string fitHistsName = "OtherBackgroundFits.root";
	std::string fitParameterValueFile = "OtherBackgroundFunctions.txt";

	// these don't do anything
	std::string parameterFits = "H++BackgroundParameterFits.root";
	std::string parameterFunctions = "H++BackgroundParameterFunctions.txt";
	remove(fitParameterValueFile.c_str());
	remove(parameterFunctions.c_str());
	
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

	std::map<std::string, std::pair<int, int>> backgroundsToRange = {
		{"Drell-Yan Background", {140, 500}},
		{"QCD Background", {200, 2000}}, // no events anyway
		{"ZZ Background", {400, 800}},
		{"TTbar Background", {70, 2000}},
		// {"TTW Background", {90, 2000}}, // almost no events on uuuu, figure out something
		// {"TTZ Background", {120, 2000}},
		// {"ZZZ Background", {200, 2000}}, // not enough to fit
		// {"WW Background", {200, 2000}}, // not enough to fit
		// {"WWW Background", {200, 2000}}, // not enough to fit
		// {"WWZ Background", {200, 2000}}, // not enough
		// {"WZ Background", {200, 2000}}, // not enough
		// {"WZZ Background", {200, 2000}}, // enough on some?
		{"Other Background", {200, 2000}},
	};
	std::map<std::string, std::vector<std::string>> fileMap =
	{
		{"Drell-Yan Background", {"DYJetsToLLM10to50", "DYJetsToLLM50"}},
		{"QCD Background", {"QCD_HT1000to1500", "QCD_HT100to200", "QCD_HT1500to2000", "QCD_HT2000toInf", "QCD_HT200to300", "QCD_HT300to500", "QCD_HT500to700", "QCD_HT50to100", "QCD_HT700to1000"}},
		{"ZZ Background", {"ZZTo2L2Nu", "ZZTo2Q2L", "ZZTo4L"}},
		{"TTbar Background", {"ST_s-channel", "ST_t-channel_antitop", "ST_t-channel_top", "ST_tW_antitop", "ST_tW_top", "TTTo2L2Nu", "TTToHadronic", "TTToSemiLeptonic"}},
		{"Other Background", {"GluGluZH", "HZJ_HToWWTo2L2Nu_ZTo2L", "TWZToLL", "WGToLNuG", "WJetsToLNu_0J", "WJetsToLNu_2J", "WJetsToLNu_2J", "WJetsToLNu_HT-100To200", "WJetsToLNu_HT-200To400", "WJetsToLNu_HT-400To600", "WJetsToLNu_HT-600To800", "WJetsToLNu_HT-70To100", "WJetsToLNu_HT-800To1200", "WJetsToLNu_NLO", "WWTo2L2Nu", "WWW", "WWZ", "WW", "WZTo2Q2L", "WZTo3LNu", "WZZ", "WZ", "ZHToMuMu", "ZHToTauTau", "ZZTo2L2Nu", "ZZTo2Q2L", "ZZTo4L", "ZZZ", "ttHJetToNonbb", "ttHTo2L2Nu", "ttHToEE", "ttHToTauTau", "ttWJets", "ttZJets"}}

	};
	Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);
   auto file = TFile::Open("/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_peter/ZZTo4L_2016.root");
    std::cout << "Loaded histogram\n";

	for (const auto& histType : histogramTypes) 
	{
		for (const auto& channel : channelTypes)
		{
			std::unordered_map<std::string, double> massValues;
			std::unordered_map<std::string, TH1*> histogramMap;
			FitFunctionCollection currentFunctions;
			std::vector<std::string> keyNames;
			for (auto fileList : fileMap)
			{
				auto background = fileList.first;
				auto range = backgroundsToRange[background];
				TH1* selectedHist = combineHists(fileList.second, channel, histType);
				if(selectedHist->GetEntries() < 1) continue;
				std::string keyName = channel + '/' + background + " " + histType;
				keyNames.push_back(keyName);

				FitFunction func = FitFunction::createFunctionOfType(FitFunction::POWER_LAW, keyName, "", range.first, range.second);
				currentFunctions.insert(func);
				histogramMap.insert({keyName, selectedHist});
			}
			fitter.setHistograms(histogramMap);
			fitter.loadFunctions(currentFunctions);
			fitter.fitFunctions();
		}
	}

}
TH1* combineHists (std::vector<std::string> fileNames, std::string channel, std::string histName)
{
	TH1* hist = nullptr;
	for (auto fileName:fileNames)
	{
		for (auto year:years)
		{
			auto fullName = path + fileName + "_" + year + ".root";
			auto file = TFile::Open(fullName.c_str());
			if (!file)
			{	
				std::cout<<"file "<<fullName<<" not found\n";
				continue;
			}
			auto directory = file -> GetDirectory(channel.c_str());
			if (!directory)
			{	
				std::cout<<"directory "<<channel<<" not found\n";
				continue;
			}
			TH1* selectedHist = directory -> Get<TH1> (histName.c_str());
			if (!selectedHist)
			{	
				std::cout<<"Histogram "<<histName<<" not found\n";
				continue;
			}
			if (!hist)
			{
				hist = dynamic_cast<TH1*>(selectedHist -> Clone());
			}
			else
			{
				hist -> Add(selectedHist);
			}
		}
	}
	return hist;
}