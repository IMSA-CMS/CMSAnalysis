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
	"eeee", "eeeu", "eueu", "eeuu", "euuu", "uuuu"
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
void HiggsBackgroundFitFromFile2()
{
	std::cout << "[INFO] Starting HiggsBackgroundFitFromFile2()" << std::endl;

	std::string fitHistsName = "OtherBackgroundFits.root";
	std::string fitParameterValueFile = "OtherBackgroundFunctions.txt";
	std::string parameterFits = "H++BackgroundParameterFits.root";
	std::string parameterFunctions = "H++BackgroundParameterFunctions.txt";

	remove(fitParameterValueFile.c_str());
	remove(parameterFunctions.c_str());

	std::map<std::string, std::pair<int, int>> backgroundsToRange = {
		{"Drell-Yan Background", {140, 500}},
		{"QCD Background", {200, 2000}}, // no events anyway
		{"ZZ Background", {100, 800}},
		{"TTbar Background", {70, 2000}},
		{"TTZ Background", {120, 2000}},
	};

	Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);

	std::cout << "[INFO] Opening ROOT file..." << std::endl;
	auto file = TFile::Open("/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_peter/ZZTo4L_2016.root");
	if (!file || file->IsZombie()) {
		std::cerr << "[ERROR] Failed to open ROOT file!" << std::endl;
		return;
	}
	std::cout << "[INFO] ROOT file opened successfully." << std::endl;

	for (const auto& histType : histogramTypes)
	{
		std::cout << "[INFO] Processing histogramType: " << histType << std::endl;

		for (const auto& channel : channelTypes)
		{
			std::cout << "[INFO]  └─ Processing channel: " << channel << std::endl;

			std::unordered_map<std::string, double> massValues;
			std::unordered_map<std::string, TH1*> histogramMap;
			FitFunctionCollection currentFunctions;
			std::vector<std::string> keyNames;

			for (auto backgroundAndRange : backgroundsToRange)
			{
				auto background = backgroundAndRange.first;
				auto range = backgroundAndRange.second;

				std::cout << "[INFO]     └─ Background: " << background << " | Range: " << range.first << "-" << range.second << std::endl;

				auto directory = file->GetDirectory(channel.c_str());
				if (!directory)
				{
					std::cerr << "[WARNING] Directory not found: " << channel << std::endl;
					continue;
				}

				TH1* selectedHist = directory->Get<TH1>(histType.c_str());
				if (!selectedHist)
				{
					std::cerr << "[WARNING] Histogram not found: " << histType << " in " << channel << std::endl;
					continue;
				}

				if (selectedHist->GetEntries() < 1)
				{
					std::cerr << "[WARNING] Empty histogram: " << histType << " in " << channel << std::endl;
					continue;
				}

				std::string keyName = channel + '/' + background + " " + histType;
				keyNames.push_back(keyName);

				std::cout << "[INFO]        → Creating fit function: " << keyName << std::endl;

				FitFunction func = FitFunction::createFunctionOfType(FitFunction::POWER_LAW, keyName, "", range.first, range.second);
				currentFunctions.insert(func);
				histogramMap.insert({keyName, selectedHist});
			}

			std::cout << "[INFO]  └─ Setting histograms and functions for " << channel << " - " << histType << std::endl;
			fitter.setHistograms(histogramMap);
			fitter.loadFunctions(currentFunctions);

			std::cout << "[INFO]  └─ Calling fitFunctions()..." << std::endl;
			fitter.fitFunctions();
			std::cout << "[INFO]  └─ Finished fitFunctions() for " << channel << " - " << histType << std::endl;
		}
	}

	std::cout << "[INFO] Finished HiggsBackgroundFitFromFile2()" << std::endl;
}