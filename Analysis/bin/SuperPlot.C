#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/ChannelLoader.hh"
#include "CMSAnalysis/Analysis/interface/FullAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsPlusPlusAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include <fstream>
#include "THStack.h"
#include "TString.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"


void SuperPlot()
{
	auto higgsAnalysis = std::make_shared<HiggsPlusPlusAnalysis>();
	//Change the string for the channel you want. For HiggsPlusPlusAnalysis, the name is the branching ratio particle + the mass target
	std::shared_ptr<Channel> leptonBackgrounds = higgsAnalysis->getChannel("Muon700");
	for(std::string processName : leptonBackgrounds->getNames()) {
		//Change this line to make the described name your signal process name.
		if(processName != "Higgs Signal") {
			leptonBackgrounds->labelProcess("background", processName);
		}
		else {
			leptonBackgrounds->labelProcess("signal", processName);
		}
	}

	//This is for making single hist graphs. Just change the process name to the one you want. Not all processes or qualities allow 2D hists.
	std::shared_ptr<Process> process = leptonBackgrounds->findProcess("Higgs Signal");
	
	//Write an extra text for the graph here
	TString extraText = "Preliminary";

	//Write axis titles here
	TString xAxisTitle = "MET [GeV]";
	TString yAxisTitle = "Events";

	//Creates the graph; to change the quality you've looking at edit the HistVariable
	//For now, don't set drawLogo to true as it's not working yet
	TCanvas *canvas = PlotFormatter::superImposedStackHistogram(leptonBackgrounds, HistVariable::MET, false, extraText, xAxisTitle, yAxisTitle);
	//TCanvas *canvas = PlotFormatter::simple1DHist(process, HistVariable::MET, true, false, extraText, xAxisTitle, yAxisTitle);
	//TCanvas *canvas = PlotFormatter::simple2DHist(process, HistVariable::InvariantMass, false, extraText, xAxisTitle, yAxisTitle);
	//TCanvas *canvas = PlotFormatter::simpleStackHist(leptonBackgrounds, HistVariable::MET, false, extraText, xAxisTitle, yAxisTitle);
	//TCanvas *canvas = PlotFormatter::superImposedHist(leptonBackgrounds, HistVariable::MET, true, false, extraText, xAxisTitle, yAxisTitle);


	//Uncomment to save a png picture in your bin folder
	//canvas->SaveAs("SuperPlot.png");
}
