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
	// "eeee", "eeeu", "eueu", "eeuu", "euuu", "uuuu"
	"ee",
	"eu",
	"uu",
};

std::vector<std::string> histogramTypes = 
{
	// "X Projection",
	// "Y Projection",
	// "Same Sign Invariant Mass",
};

// run in batch mode for faster processing: root -b HiggsBackgroundFit.C+
void HiggsBackgroundFit()
{
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
		{"ZZ Background", {100, 800}},
		{"TTbar Background", {70, 2000}},
		// {"TTW Background", {90, 2000}}, // almost no events on uuuu, figure out something
		{"TTZ Background", {120, 2000}},
		// {"ZZZ Background", {200, 2000}}, // not enough to fit
		// {"WW Background", {200, 2000}}, // not enough to fit
		// {"WWW Background", {200, 2000}}, // not enough to fit
		// {"WWZ Background", {200, 2000}}, // not enough
		// {"WZ Background", {200, 2000}}, // not enough
		// {"WZZ Background", {200, 2000}}, // enough on some?
		// {"Other Background", {200, 2000}},
	};

	Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);
    std::shared_ptr<HiggsCompleteAnalysis> analysis = std::make_shared<HiggsCompleteAnalysis>();
    std::cout << "Loaded histogram\n";

	for (const auto& histType : histogramTypes) 
	{
		for (const auto& channel : channelTypes)
		{
			std::unordered_map<std::string, double> massValues;
			std::unordered_map<std::string, TH1*> histogramMap;
			FitFunctionCollection currentFunctions;
			auto targetChannel = analysis->getChannel(channel);
			std::vector<std::string> keyNames;
			for (auto backgroundAndRange : backgroundsToRange)
			{
				auto background = backgroundAndRange.first;
				auto range = backgroundAndRange.second;
				try 
				{
					auto process = targetChannel->findProcess(background);
				} catch (std::runtime_error&) 
				{
					continue;
				}
				auto process = targetChannel->findProcess(background);
				auto histVar = HistVariable(histType);

				TH1* selectedHist = process->getHist(histVar, true);
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
