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
	auto higgsAnalysis = std::make_shared<HiggsComparisonAnalysis>();
	//Change extra text here (keep drawLogo to false for now)
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary");
	//Change the string for the channel you want. For HiggsPlusPlusAnalysis, the name is the branching ratio particle + the mass target
	std::shared_ptr<Channel> leptonBackgrounds = higgsAnalysis->getChannel("Muon700");
	for(std::string processName : leptonBackgrounds->getNames()) {
		//Change this line to make the described name your signal process name.
		if(processName != "Old Higgs") {
			leptonBackgrounds->labelProcess("background", processName);
		}
		else {
			leptonBackgrounds->labelProcess("signal", processName);
		}
	}

	//This is for making single hist graphs. Just change the process name to the one you want. Not all processes or qualities allow 2D hists.
	std::shared_ptr<Process> process = leptonBackgrounds->findProcess("New Higgs");

	//Write axis titles here
	TString xAxisTitle = "MET [GeV]";
	TString yAxisTitle = "Events";

	//Creates the graph
	//For now, don't set drawLogo to true as it's not working yet
	//Choices for plotting are GenSim Same Sign Inv Mass, Same Sign Inv Mass, Invariant Mass, GenSim pT, pT, Eta, Phi, MET (caps matter)
	//TCanvas *canvas = plotFormatter->superImposedStackHist(leptonBackgrounds, "Invariant Mass"	, xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simple1DHist(process, "MET", true, xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simple2DHist(process, "Invariant Mass", xAxisTitle, yAxisTitle);
	//TCanvas *canvas = plotFormatter->simpleStackHist(leptonBackgrounds, "MET", xAxisTitle, yAxisTitle);
	TCanvas *canvas = plotFormatter->superImposedHist(leptonBackgrounds, "MET", xAxisTitle, yAxisTitle);


	//Uncomment to save a png picture in your bin folder
	//canvas->SaveAs("SuperPlot.png");
}
