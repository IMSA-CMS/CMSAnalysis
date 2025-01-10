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
};

std::vector<std::string> histogramTypes = 
{
	"_Reco Same Sign Invariant Mass"
};

// run in batch mode for faster processing: root -b HiggsBackgroundFit.C+
void HiggsBackgroundFit()
{
	const double min = 250;
	const double max = 2000;

	std::string fitHistsName = "H++Backgrounds.root";
	std::string fitParameterValueFile = "H++BackgroundFunctions.txt";

	// these don't do anything
	std::string parameterFits = "H++BackgroundParameterFits.root";
	std::string parameterFunctions = "H++BackgroundParameterFunctions.txt";
	remove(fitParameterValueFile.c_str());
	remove(parameterFunctions.c_str());
	
	std::vector<std::string> backgrounds = {"t#bar{t}, Multiboson Background", "Drell-Yan Background", "QCD Background", "ZZ Background", "All Background"};

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
			for (size_t i = 0; i < backgrounds.size(); ++i) 
			{
				try 
				{
					auto process = targetChannel->findProcess(backgrounds[i]);
				} catch (std::runtime_error&) 
				{
					continue;
				}
				auto process = targetChannel->findProcess(backgrounds[i]);
				auto histVar = HistVariable("Same Sign Invariant Mass");

				TH1* selectedHist = process->getHist(histVar, true);
				if(selectedHist->GetEntries() < 1) continue;
				std::string keyName = channel + '/' + backgrounds[i] + histType;
				keyNames.push_back(keyName);

				FitFunction func = FitFunction::createFunctionOfType(FitFunction::POWER_LAW, keyName, "", min, max);
				currentFunctions.insert(func);
				histogramMap.insert({keyName, selectedHist});
			}
			fitter.setHistograms(histogramMap);
			fitter.loadFunctions(currentFunctions);
			fitter.fitFunctions();
			for (std::string keyName : keyNames) 
			{
				std::cout << "Key: " << keyName << std::endl;
				std::cout << currentFunctions.get(keyName) << std::endl;
			}
		}
	}

}
