
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsComparisonAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/DarkPhotonCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/DarkPhotonNanoAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/DarkPhotonInputAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/DarkPhotonNoCutAnalysis.hh"
#include <fstream>
#include "THStack.h"
#include "TString.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TSystem.h"


void SuperPlot()
{
	//std::vector<double> massTargets {900};
	//auto DarkPhotonAnalysis = std::make_shared<DarkPhotonCompleteAnalysis>(inputAnalysisPath, "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections_config1.txt"); // BDT Output Analysis (LeptonJetMLOutput)
	//auto DarkPhotonAnalysis = std::make_shared<DarkPhotonInputAnalysis>(inputAnalysisPath); // Variable Isolation Plots
	// auto NanoAnalysis = std::make_shared<DarkPhotonNanoAnalysis>(15, 17);
	//auto NoCutAnalysis = std::make_shared<DarkPhotonNoCutAnalysis>(inputAnalysisPath); // When control region filter disabled
	//Change extra text here (keep drawLogo to false for now)
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "Private Work (CMS Simulation)");
	plotFormatter->setUpperMasslimit(.5);
	plotFormatter->setNumBins(1);
	plotFormatter->setFirstBin(-1);
	//Change the string for the channel you want, if using one channel. Otherwise use the loop.
	
	double massTarget = 1400;
	
	//std::shared_ptr<Channel> leptonBackgrounds = higgsAnalysis->getChannel("eeee" + std::to_string((int) massTarget));
	
	//SINGLE CHANNEL CODE HERE
	// for(std::string processName : leptonBackgrounds->getNames()) {
	// 	//std::cout << processName << std::endl;
	// 	//Change this line to make the described name your signal process name.
	// 	if(processName == "Dark Photon Signal") {
	// 		leptonBackgrounds->labelProcess("signal", processName);
	// 	}
	// 	else if(processName == "Data") { //This line is only used for complete plots
	// 		leptonBackgrounds->labelProcess("data", processName);
	// 	}
	// 	else {
	// 		leptonBackgrounds->labelProcess("background", processName);
	// 	}
	// }
	//ALL CHANNEL CODE HERE

	//#std::vector<std::shared_ptr<Channel>> channels = DarkPhotonAnalysis->getChannels();
	//#std::vector<std::shared_ptr<Channel>> channels = NanoAnalysis->getChannels();
	//#std::vector<std::shared_ptr<Channel>> channels = InputAnalysis->getChannels();
	//std::vector<std::shared_ptr<Channel>> channels = DarkPhotonAnalysis->getChannels();
	HiggsCompleteAnalysis analysis;
	auto channels = analysis.getChannels();
	for(std::shared_ptr<Channel> channel : channels) {
	for(std::string processName : channel->getNames()) {
			//std::cout << processName << std::endl;
			//Change this line to make the described name your signal process name.
			if(processName == "Dark Photon Signal") {
				channel->labelProcess("signal", processName);
			}
			// "Monte Carlo Data"
			else if(processName == "Data") { //This line is only used for complete plots
				channel->labelProcess("data", processName);
			}
			else {
				channel->labelProcess("background", processName);
			}
		}
	}
	//This is for making single hist graphs. Just change the process name to the one you want. Not all processes or qualities allow 2D hists.
	//std::shared_ptr<Process> process = leptonBackgrounds->findProcess("Data");
 
	std::string plotName = "_1st Highest mu- Pt";
	//Write axis titles here
	TString xAxisTitle = plotName;
	TString yAxisTitle = "Events";

	//Creates the graph
	//For now, don't set drawLogo to true as it's not working yet
	//Choices for plotting are GenSim Same Sign Inv Mass, Same Sign Inv Mass, Invariant Mass, GenSim pT, pT, Eta, Phi, MET (caps matter)
	//TCanvas *canvas = plotFormatter->superImposedStackHist(leptonBackgrounds, "Invariant Mass", xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simple1DHist(process, "Same Sign Inv Mass", false, xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simple2DHist(process, "Invariant Mass", xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simpleStackHist(leptonBackgrounds, "Same Sign Inv Mass", xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->superImposedHist(leptonBackgrounds, "MET", xAxisTitle, yAxisTitle);
	
	/*
	std::shared_ptr<FullAnalysis> analysis = DarkPhotonAnalysis;
	std::string channelName;
	std::string histvariable = "Invariant Mass";
	std::shared_ptr<Channel> processes = 0;
	processes = channels.at(0);
	std::vector<std::string> backgroundNames = processes->getNamesWithLabel("background");
	std::vector<TH1*> backgroundHists;
	for(std::string name : backgroundNames) {
        backgroundHists.push_back(analysis->getDecayHist(histvariable, name, massTarget, true, channelName));
	}
	*/

	// std::cout << "here\n";
	// return;

	//#TCanvas *canvas = plotFormatter->completePlot(DarkPhotonAnalysis, "LeptonJetMLOutput High Mass and Different Sign", xAxisTitle, yAxisTitle, true, false, "0.3");
	//#TCanvas *canvas = plotFormatter->completePlot(DarkPhotonAnalysis, "LeptonJetMLOutput High Mass and Different Sign", xAxisTitle, yAxisTitle, true, false, "0.3");
	//#TCanvas *canvas = plotFormatter->completePlot(InputAnalysis, plotName, xAxisTitle, yAxisTitle, true, false, "0.3");
	auto nameVar = HistVariable(plotName);
	auto ana = std::make_shared<HiggsCompleteAnalysis>();
	TCanvas *canvas = plotFormatter->completePlot(ana, nameVar, xAxisTitle, yAxisTitle, true, false, "uu");

	//TCanvas *canvas = plotFormatter->simpleAnalysisHist(backgroundHists, );

	//Uncomment to save a png picture in your bin folder
	//canvas->SaveAs(outFile.c_str());
}
