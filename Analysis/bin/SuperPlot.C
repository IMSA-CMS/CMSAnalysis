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
#include <fstream>
#include "THStack.h"
#include "TString.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TSystem.h"


void SuperPlot()
{
	auto higgsAnalysis = std::make_shared<HiggsCompleteAnalysis>();
	//Change extra text here (keep drawLogo to false for now)
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary");
	//Change the string for the channel you want, if using one channel. Otherwise use the loop.
	std::shared_ptr<Channel> leptonBackgrounds = higgsAnalysis->getChannel("none900");
	//SINGLE CHANNEL CODE HERE
	// for(std::string processName : leptonBackgrounds->getNames()) {
	// 	//std::cout << processName << std::endl;
	// 	//Change this line to make the described name your signal process name.
	// 	if(processName == "Higgs Signal") {
	// 		leptonBackgrounds->labelProcess("signal", processName);
	// 	}
	// 	else if(processName == "Higgs Data") { //This line is only used for complete plots
	// 		leptonBackgrounds->labelProcess("data", processName);
	// 	}
	// 	else {
	// 		leptonBackgrounds->labelProcess("background", processName);
	// 	}
	// }
	//ALL CHANNEL CODE HERE
	std::vector<std::shared_ptr<Channel>> channels = higgsAnalysis->getChannels();
	for(std::shared_ptr<Channel> channel : channels) {
		for(std::string processName : channel->getNames()) {
			//std::cout << processName << std::endl;
			//Change this line to make the described name your signal process name.
			if(processName == "Higgs Signal") {
				channel->labelProcess("signal", processName);
			}
			else if(processName == "Higgs Data") { //This line is only used for complete plots
				channel->labelProcess("data", processName);
			}
			else {
				channel->labelProcess("background", processName);
			}
		}
	}

	//This is for making single hist graphs. Just change the process name to the one you want. Not all processes or qualities allow 2D hists.
	std::shared_ptr<Process> process = leptonBackgrounds->findProcess("Higgs Data");

	//Write axis titles here
	TString xAxisTitle = "SSDL [GeV]";
	TString yAxisTitle = "Events";

	//Creates the graph
	//For now, don't set drawLogo to true as it's not working yet
	//Choices for plotting are GenSim Same Sign Inv Mass, Same Sign Inv Mass, Invariant Mass, GenSim pT, pT, Eta, Phi, MET (caps matter)
	//TCanvas *canvas = plotFormatter->superImposedStackHist(leptonBackgrounds, "Invariant Mass", xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simple1DHist(process, "Same Sign Inv Mass", false, xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simple2DHist(process, "Invariant Mass", xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simpleStackHist(leptonBackgrounds, "Same Sign Inv Mass", xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->superImposedHist(leptonBackgrounds, "MET", xAxisTitle, yAxisTitle);
	TCanvas *canvas = plotFormatter->completePlot(leptonBackgrounds, "Same Sign Inv Mass", xAxisTitle, yAxisTitle);

	//Uncomment to save a png picture in your bin folder
	//canvas->SaveAs("SuperPlot.png");
}
