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
#include "TObjString.h"
#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#define _USE_MATH_DEFINES



std::string path = "/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_MY/";
std::string processedPath = "/uscms/home/bhenning/nobackup/HiggsWithSystematics1/";

TH1* combineHists (std::vector<std::string> fileNames, std::string channel, std::string histName);
std::vector<std::string> years = {"2016", "2017", "2018"};
std::vector<std::string> channelTypes =
{
"0tau", "1tau", "2tau", "3tau"
//"eeem", "emem", 
//"eemm", "emmm", 

};

std::vector<std::string> histogramTypes = 
{
	"h_mDCH1",
	"h_mDCH2",
};

double getBranchingRatio(const std::string &channel) //const
{
    std::unordered_map<std::string, double> originalRatios = {{"ee", 3.0 / 2}, {"eu", 3.0 / 4}, {"uu", 3.0 / 2},
                                                              {"et", 3.0 / 4}, {"ut", 3.0 / 4}, {"tt", 3.0 / 2}};

    std::string firstPair = channel.substr(0, 2);
    std::string secondPair = channel.substr(2, 2);

    return originalRatios[firstPair] * originalRatios[secondPair];
}

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

	CrossSectionReader crossSectionReader(Utility::getBasePath() + "DataCollection/bin/crossSections.txt");

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
			std::string correctedChannel = Utility::substitute(channel, "m", "u");
			for (size_t i = 0; i < masses.size(); ++i) 
			{
				std::cerr << "mass: " << masses[i] << std::endl;
				// update this to use all gensim decays, not just the same as reco
				TH1* selectedHist = combineHists(std::vector<std::string>{"HppM" + std::to_string(masses[i])}, channel, histType);
				if (!selectedHist)
				{
					std::cerr << "Histogram for mass " << masses[i] << " not found, skipping\n";
					continue;
				}
				double crossSection = crossSectionReader.getCrossSection("Higgs4l" + std::to_string(masses[i]));
				double luminosity = 137000; //in pb^-1
				//int eventsInHist = selectedHist -> Integral();
				double eventsInHist = selectedHist->Integral();
				std::string filename = processedPath + "Higgs" + std::to_string(masses[i]) + ".root";
				TFile* processedFile = TFile::Open(filename.c_str());
				std::string channelAdjusted = Utility::substitute(channel, "m", "u");
				//auto number = processedFile -> Get<TObjString>(("GenSim " + channelAdjusted).c_str());
				auto number = processedFile->Get<TObjString>("NEvents");
				int totalGeneratedEvents = std::stoi(number -> GetString().Data());
				//int totalGeneratedEvents = 1;
				//double efficiency = static_cast<double>(eventsInHist) / totalGeneratedEvents;
				double efficiency = 1; // Efficiency is already accounted for in the histogram scaling, so we set it to 1 here to avoid double counting
				//double branchRatioAdjustment = getBranchingRatio(channelAdjusted);
				double expectedEvents = crossSection * luminosity * efficiency;
				selectedHist -> Scale(expectedEvents);
				
				std::string keyName = correctedChannel + "/" + std::to_string(masses[i]) + '_' + histType;
				min = masses[i] - 200;
				max = masses[i] + 200;

				std::cout << "Cross section: " << crossSection << std::endl;
				std::cout << "Selected events: " << eventsInHist << std::endl;
				std::cout << "Efficiency: " << efficiency << std::endl;
				std::cout << "Total generated events: " << totalGeneratedEvents << std::endl;
				//std::cout << "Branching ratio adjustment: " << branchRatioAdjustment << std::endl;
				std::cout << "Expected events: " << expectedEvents << std::endl;
				std::cout << "Scaling factor: " << expectedEvents / selectedHist -> Integral() << std::endl;
				std::cout << "Expected Signal: " << selectedHist -> Integral() << std::endl;
		


				FitFunction func = FitFunction::createFunctionOfType(FitFunction::FunctionType::DoubleSidedCrystalBall, keyName, "", min, max, keyName);
				std::cout << __LINE__ << std::endl;
				currentFunctions.insert(func);
				histogramMap.insert({keyName, selectedHist});
				massValues.insert({keyName, masses[i]});
				std::cout << __LINE__ << std::endl;
				// file->Close();
				// selectedHist->Draw();
				// std::string wait;
				// std::cin >> wait;
			}
			std::cout << __LINE__ << std::endl;
			//fitter.setHistograms(histogramMap);
			fitter.loadFunctions(currentFunctions);
			fitter.fitFunctions(histogramMap);
			HistVariable histVar(HistVariable::VariableType::InvariantMass, "", histType == "h_mll1", histType == "h_mll2");

			fitter.parameterizeFunctions(massValues, correctedChannel, correctedChannel, "Mass", histVar);
		}
	}
}
TH1* combineHists (std::vector<std::string> fileNames, std::string channel, std::string histName)
{
	std::unordered_map<std::string, int> codeMap = {{"eeee", 1}, {"eeem", 2}, {"eemm", 4}, {"emem", 7}, {"emmm", 9}, {"mmmm", 16}};
	TH1* hist = nullptr;
	int totalEvents = 0;
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
			auto selectedHist = directory->Get<TH1>("h_mDCH1");
			// auto tree = directory -> Get<TTree> ("Events");
			// std::string drawCommand = "mll1>>" + histName + "(2000, 0, 2000)";
			// std::string cutCommand = std::string("gen_cat==") + std::to_string(codeMap [channel]);
			// tree -> Draw (drawCommand.c_str(), cutCommand.c_str());
			// TH1* selectedHist = dynamic_cast <TH1*> (gDirectory -> FindObject(histName.c_str()));
			auto nEvents = file->Get<TH1>("hNWEvts");
			if (!selectedHist)
	
			{	
				std::cout<<"Histogram "<<histName<<" not found\n";
				continue;
			}
			std::cout << "nEvents" << nEvents->GetBinContent(1) << std::endl;
			std::cout << "Integral of selected histogram: " << selectedHist->Integral() << std::endl;
			totalEvents += nEvents->GetBinContent(1);
			//totalEvents = 1;

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
	hist -> Scale(1.0 / totalEvents);
	return hist;
}
