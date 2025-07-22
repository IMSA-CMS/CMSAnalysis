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
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#define _USE_MATH_DEFINES

// this takes existing fit functions and scales them to another channel
// as of now, used primarily for Drell-Yan due to low statistics

std::map<std::string, std::string> channelMap =
{
	{"ee", "eeee"},
	{"eu", "eueu"},
	{"uu", "uuuu"},
};

std::map<std::string, std::string> histogramMap = 
{
	{"Same Sign Invariant Mass", "X Projection"}
};

// maybe should be different ranges for different channels?
std::map<std::string, std::pair<int, int>> backgroundsToRange = {
	{"Drell-Yan Background", {0, 2000}}
};

// run in batch mode for faster processing: root -b HiggsBackgroundFit.C+
void FitFunctionMapping()
{
	std::string fitParameterValueFile = Utility::getBasePath() + "Analysis/bin/fitting/H++BackgroundFunctions2.txt";
	std::string fitOutputFile = Utility::getBasePath() + "Analysis/bin/fitting/Drell-YanMappedFits2.root";
	auto fitRootFile = TFile::Open(fitOutputFile.c_str(), "RECREATE");
    std::shared_ptr<HiggsCompleteAnalysis> analysis = std::make_shared<HiggsCompleteAnalysis>();
    std::cout << "Loaded histogram\n";

	std::map<std::string, std::map<std::string, TF1*>> backgroundFits; // channel to [background to function]
	for (const auto& channel : channelMap)
	{
		auto origChannel = channel.first;
		auto newChannel = channel.second;
		auto fitDirectory = fitRootFile->mkdir(newChannel.c_str());

		auto targetChannel = analysis->getChannel(newChannel);
		for (const auto& histType : histogramMap) 
		{
			auto origHistType = histType.first;
			auto newHistType = histType.second;
			// std::unordered_map<std::string, double> massValues;
			// std::unordered_map<std::string, TH1*> histogramMap;
			// FitFunctionCollection currentFunctions;
			// std::vector<std::string> keyNames;

			for (auto backgroundAndRange : backgroundsToRange)
			{
				// getting the fit function from the file
				auto background = backgroundAndRange.first;
				auto functionName = origChannel + "/" + background + " " + origHistType;

				// should probably extract this out to a separate executable to avoid duplication
				std::ifstream backgroundFile(fitParameterValueFile); 
				std::string line;
				TF1* powerLaw = new TF1("", "[0]*(x-[1])^[2]", 0, 2000);
				while (std::getline(backgroundFile, line)){
					if(line == ("Name: " + functionName)){
						for(int i = 0; i < 5; ++i){
							std::getline(backgroundFile, line);
						}
						std::string filler, p0, p1, p2;
						backgroundFile >> filler >> p0 >> p1 >> p2;
						powerLaw->SetParameter(0, std::stod(p0));
						powerLaw->SetParameter(1, std::stod(p1));
						powerLaw->SetParameter(2, std::stod(p2));
						backgroundFits[origChannel][background] = powerLaw;
						break;
					}
				}

				// scaling to the new channel
				try
				{
					auto process = targetChannel->findProcess(background);
				} catch (std::runtime_error&) 
				{
					continue;
				}

				auto histName = background + " " + newHistType;
				TCanvas *canvas = new TCanvas(histName.c_str(), histName.c_str(),0,0,1500,500);
				canvas->SetLogy();

				auto process = targetChannel->findProcess(background);
				auto histVar = HistVariable(newHistType);

				std::cout << "Channel: " << newChannel << " Background: " << background << std::endl;
				TH1* selectedHist = process->getHist(histVar, true);

				auto range = backgroundsToRange[background];

				auto origEvents = powerLaw->Integral(range.first, range.second);
				// this doesn't work w/ bin size != 1
				// can do some math to get the correct indices
				auto newEvents = selectedHist->Integral(range.first, range.second);
				auto scaleFactor = newEvents / origEvents;
				powerLaw->SetParameter(0, powerLaw->GetParameter(0) * scaleFactor);
				
				std::cout << "New Power Law: " << powerLaw->GetParameter(0) << " " << powerLaw->GetParameter(1) << " " << powerLaw->GetParameter(2) << std::endl;
				selectedHist->Draw();
				// powerLaw->SetMinimum(50);
				powerLaw->DrawF1(50, 2000, "SAME");
				fitDirectory->WriteObject(canvas, histName.c_str());
				canvas->Close();
			}
			// fitter.setHistograms(histogramMap);
			// fitter.loadFunctions(currentFunctions);
			// fitter.fitFunctions();
			// for (std::string keyName : keyNames) 
			// {
			// 	std::cout << "Key: " << keyName << std::endl;
			// 	std::cout << currentFunctions.get(keyName) << std::endl;
			// }
		}
	}
	fitRootFile->Close();

}
