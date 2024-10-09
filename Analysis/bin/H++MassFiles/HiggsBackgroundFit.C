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
	"ee",
};

std::vector<std::string> histogramTypes = 
{
	"_Reco Same Sign Invariant Mass"
};

void HiggsBackgroundFit()
{
	const double min = 0;
	const double max = 2000;

	std::string fitHistsName = "H++BackgroundFits.root";
	std::string fitParameterValueFile = "H++BackgroundFunctions.txt";
	std::string parameterFits = "H++BackgroundParameterFits.root";
	std::string parameterFunctions = "H++BackgroundParameterFunctions.txt";
	std::vector<std::string> backgrounds = {"t#bar{t}, WW, WZ, ZZ Background", "Drell-Yan Background", "QCD Background", "ZZ Background"};

	Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);
    std::shared_ptr<HiggsCompleteAnalysis> analysis = std::make_shared<HiggsCompleteAnalysis>();
    std::cout << "Loaded histogram\n";
    std::vector<std::string> paramNames = {"alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma", "norm"};

	for (const auto& histType : histogramTypes) 
	{
		for (const auto& channel : channelTypes) 
		{
			std::unordered_map<std::string, double> massValues;
			std::unordered_map<std::string, TH1*> histogramMap;
			FitFunctionCollection currentFunctions;
			std::vector<std::string> actualParams;
			auto targetChannel = analysis->getChannel(channel);
			for (size_t i = 0; i < paramNames.size(); ++i) {
				actualParams.push_back(channel + '_' + paramNames[i]);
			}

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
				auto histVar = HistVariable(histType + " " + channel, histType + "__hists/" + channel + histType);

				TH1* selectedHist = process->getHist(histVar, true);
				if(selectedHist->GetEntries() < 1) continue;
				std::string keyName = backgrounds[i] + '_' + channel + '_' + histType;

				FitFunction func = FitFunction::createFunctionOfType(FitFunction::POWER_LAW, keyName, "", min, max);
				currentFunctions.insert(func);
				histogramMap.insert({keyName, selectedHist});
				// massValues.insert({keyName, backgrounds[i]});

				// file->Close();
				// selectedHist->Draw();
				// std::string wait;
				// std::cin >> wait;
			}
			fitter.histograms = histogramMap;
			fitter.loadFunctions(currentFunctions);
			fitter.fitFunctions();
			// fitter.parameterizeFunctions(massValues, actualParams);
		}
	}

}
