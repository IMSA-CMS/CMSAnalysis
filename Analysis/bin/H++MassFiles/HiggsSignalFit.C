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
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#define _USE_MATH_DEFINES

std::vector<std::string> channelTypes =
{
	"eeee",
	"eeeu",
	"eueu",
	"eeuu",
	"euuu",
	"uuuu"
};

std::vector<std::string> histogramTypes = 
{
	"Same Sign Invariant Mass"
};

// run in batch mode for faster processing: root -b HiggsSignalFit.C+
void HiggsSignalFit() 
{
	const double min = 0;
	const double max = 2000;

	std::string fitHistsName = "H++SignalFits.root";
	std::string fitParameterValueFile = "H++SignalFunctions.txt";
	std::string parameterFits = "H++SignalParameterFits.root";
	std::string parameterFunctions = "H++SignalParameterFunctions.txt";
	std::vector<int> masses = {500, 600, 700, 800, 900, 1100, 1200, 1300, 1400, 1500};

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
				actualParams.push_back(channel + '/' + paramNames[i]);
			}

			for (size_t i = 0; i < masses.size(); ++i) 
			{
				auto process = targetChannel->findProcess("Higgs signal " + channel + " " + std::to_string(masses[i]));
				TH1* selectedHist = process->getHist(HistVariable("Same Sign Invariant Mass"), true);
				std::string keyName = channel + "/" + std::to_string(masses[i]) + '_' + histType;

				FitFunction func = FitFunction::createFunctionOfType(FitFunction::DOUBLE_SIDED_CRYSTAL_BALL, keyName, "", min, max);
				currentFunctions.insert(func);
				histogramMap.insert({keyName, selectedHist});
				massValues.insert({keyName, masses[i]});

				// file->Close();
				// selectedHist->Draw();
				// std::string wait;
				// std::cin >> wait;
			}
			fitter.setHistograms(histogramMap);
			fitter.loadFunctions(currentFunctions);
			fitter.fitFunctions();
			fitter.parameterizeFunctions(massValues, actualParams);
		}
	}

}
