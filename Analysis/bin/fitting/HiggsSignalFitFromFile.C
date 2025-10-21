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
#include "TTree.h"
#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#define _USE_MATH_DEFINES

std::string path = "/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_peter/";
TH1* combineHists (std::vector<std::string> fileNames, std::string channel, std::string histName);
std::vector<std::string> years = {"2016", "2017", "2018"};
std::vector<std::string> channelTypes =
{
"eeee", //"eeem", "emem", 
//"eemm", "emmm", 
"mmmm"
};

std::vector<std::string> histogramTypes = 
{
	"h_mll1",
	"h_mll2",
};

// run in batch mode for faster processing: root -b HiggsSignalFit.C+
void HiggsSignalFitFromFile() 
{
	 double min = 0;
	 double max = 2000;

	std::string fitHistsName = "H++SignalFits.root";
	std::string fitParameterValueFile = "H++SignalFunctions.txt";
	std::string parameterFits = "H++SignalParameterFits.root";
	std::string parameterFunctions = "H++SignalParameterFunctions.txt";
	
	remove(fitParameterValueFile.c_str());
	remove(parameterFunctions.c_str());
	std::vector<int> masses = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};

	Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);

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
			for (size_t i = 0; i < paramNames.size(); ++i) {
				actualParams.push_back(channel + '/' + paramNames[i] + "_" + histType);
			}

			for (size_t i = 0; i < masses.size(); ++i) 
			{
				std::cerr << "mass: " << masses[i] << std::endl;
				// update this to use all gensim decays, not just the same as reco
				TH1* selectedHist = combineHists(std::vector<std::string>{"HppM" + std::to_string(masses[i])}, channel, histType);
				selectedHist -> Scale(1/selectedHist -> Integral());
				std::string keyName = channel + "/" + std::to_string(masses[i]) + '_' + histType;
				min = masses[i] - 200;
				max = masses[i] + 200;

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
TH1* combineHists (std::vector<std::string> fileNames, std::string channel, std::string histName)
{
	std::unordered_map<std::string, int> codeMap = {{"eeee", 1}, {"eeem", 2}, {"eemm", 4}, {"emem", 7}, {"emmm", 9}, {"mmmm", 16}};
	TH1* hist = nullptr;
	for (auto fileName:fileNames)
	{
		for (auto year:years)
		{
			std::cout << "Starting loop\n";
			auto fullName = path + fileName + "_" + year + ".root";
			std::cout << "trying file " << fullName << std::endl;
			auto file = TFile::Open(fullName.c_str());
			std::cout<<"opening file "<<fileName<< std::endl;
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
			auto tree = directory -> Get<TTree> ("Events");
			std::string drawCommand = "mll1>>" + histName + "(2000, 0, 2000)";
			std::string cutCommand = std::string("gen_cat==") + std::to_string(codeMap [channel]);
			tree -> Draw (drawCommand.c_str(), cutCommand.c_str());
			TH1* selectedHist = dynamic_cast <TH1*> (gDirectory -> FindObject(histName.c_str()));
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
