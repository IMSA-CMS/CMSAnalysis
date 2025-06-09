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

std::vector<std::string> channelTypes =
{
	"uuuu",
	// "eeeu", "eueu", "eeuu", "euuu", "uuuu"
};

void simpleAnalysis()
{
	const double min = 500; // kind of sucks, but some params are undefined below this point cuz of the fits
	const double max = 2000;

	auto windowSize = 60.0;
	auto analysis = std::make_shared<HiggsCompleteAnalysis>();
	std::vector<int> masses = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};
	std::vector<std::string> backgrounds = {"t#bar{t}, Multiboson Background", "Drell-Yan Background", "QCD Background", "ZZ Background"};
	std::vector<std::string> paramNames = {"alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma", "norm"};

	std::ifstream backgroundFile(Utility::getBasePath() + "Analysis/bin/fitting/H++BackgroundFunctions.txt"); 
	std::string line;
	std::map<std::string, std::map<std::string, TF1*>> backgroundFits; // channel to [background to function]
	while (std::getline(backgroundFile, line)){
		for(auto channel: channelTypes){
			if(line.find(channel) == std::string::npos) continue; // this is bad, will "double count" if we do 2/3-lepton processes
			for(auto background: backgrounds){
				if(line.find(background) == std::string::npos) continue;
				for(int i = 0; i < 5; ++i){
					std::getline(backgroundFile, line);
				}
				std::string filler, p0, p1, p2;
				backgroundFile >> filler >> p0 >> p1 >> p2;
				TF1* powerLaw = new TF1("", "[0]*(x-[1])^[2]", min, max);
				powerLaw->SetParameter(0, std::stod(p0));
				powerLaw->SetParameter(1, std::stod(p1));
				powerLaw->SetParameter(2, std::stod(p2));
				backgroundFits[channel][background] = powerLaw;

			}
		}
	}

	std::ifstream signalFile(Utility::getBasePath() + "Analysis/bin/fitting/H++SignalParameterFunctions2.txt"); 
	std::map<std::string, std::map<std::string, TF1*>> signalParameterizations; // channel to [background to function]
	while (std::getline(signalFile, line)){
		for(auto channel: channelTypes){
			if(line.find(channel) == std::string::npos) continue;
			for(auto param: paramNames){
				if(line.find(param) == std::string::npos) continue;
				for(int i = 0; i < 5; ++i){
					std::getline(signalFile, line);
				}
				std::string filler, p0, p1, p2;
				signalFile >> filler >> p0 >> p1 >> p2;
				TF1* powerLaw = new TF1("", "[0]*(x-[1])^[2]", min, max);
				powerLaw->SetParameter(0, std::stod(p0));
				powerLaw->SetParameter(1, std::stod(p1));
				powerLaw->SetParameter(2, std::stod(p2));
				signalParameterizations[channel][param] = powerLaw;

			}
		}
	}
	
	auto card = std::fstream(Utility::getBasePath() + "Output/Simple/card.txt", std::ios_base::out);

	for(auto c: channelTypes) {
		std::cout << "channel: " << c << std::endl;
		auto channel = analysis->getChannel(c);
		auto dataProcess = channel->findProcess("Data");
		TH1* signalHist = dataProcess->getHist(HistVariable("Same Sign Invariant Mass"));

		// auto dataProcess = channel->findProcess("Data");
		// TH1* signalHist = dataProcess->getHist(HistVariable("Same Sign Invariant Mass"));
		for(int lower = min; lower + windowSize < max; lower += windowSize / 2){
			int upper = lower + windowSize;
			std::cout << lower << " to " << upper << std::endl;
			
			// this is cuz data is binned by 2GeV
			int lowerBin = (lower / 2) + 1;
			int upperBin = upper / 2;
			std::cout << "Not Data Events: " << signalHist->Integral(lowerBin, upperBin) << std::endl;

			double backgroundEvents = 0;
			for(auto background: backgrounds) {
				auto function = backgroundFits[c][background];
				auto events = (function ? function->Integral(lower, upper) : 0);
				std::cout << background << " Events: " << events << std::endl;
				backgroundEvents += events;
			}
			std::cout << "Total Background Events: " << backgroundEvents << std::endl;

			FitFunction func = FitFunction::createFunctionOfType(FitFunction::DOUBLE_SIDED_CRYSTAL_BALL, "", "", 0, 2000);
			int middle = (upper + lower) / 2;
			for(unsigned i = 0; i < paramNames.size(); i++) {
				auto param = paramNames[i];
				auto paramFunction = signalParameterizations[c][param];
				double value = paramFunction->Eval(middle);
				func.getFunction()->SetParameter(i, value);
			}
			double signalEvents = func.getFunction()->Integral(lower, upper, 1e-11); // adjust this as low as possible? or error doesnt really matter tbh
			std::cout << "Total Signal Events: " << signalEvents << std::endl;
		}
	}
	return;
	// auto channelTest = analysis->getChannel("uuuu");
	// auto higgsProcess = channelTest->findProcess("Data");
	// TH1* signalHist = higgsProcess->getHist(HistVariable("Same Sign Invariant Mass"));
	// while(true) {
	// 	double x1, x2;
	// 	std::cin >> x1 >> x2;
	// 	std::cout << std::endl << signalHist->Integral(x1, x2) << std::endl;
	// }
	// signalHist->DrawCopy();
	// signalHist->SaveAs((Utility::getBasePath() + "Output/Simple/Data.root").c_str());

	// return;
	

	for(auto channel: channelTypes){
		std::cout << channel << std::endl;
	}

	// std::cout << "Done loading HCA\n";
	// for (int mass : masses) 
	// {
	// 	// std::cout << "Current mass " << mass << '\n';
	// 	auto channelTest = analysis->getChannel("eeuu");
	// 	auto higgsProcess = channelTest->findProcess("Higgs Signal " + std::to_string(mass));
	// 	TH1* signalHist = higgsProcess->getHist("Same Sign Inv Mass");
	// 	signalHist->DrawCopy();
	// }

	// auto backgroundChannel = analysis->getChannel("eeuu");
	// auto zz_background = backgroundChannel->findProcess("ZZ Background");
	// zzHist->DrawCopy();
}
