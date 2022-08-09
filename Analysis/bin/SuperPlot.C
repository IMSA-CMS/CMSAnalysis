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
#include <fstream>
#include "THStack.h"
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
	THStack* hist1 = leptonBackgrounds->getStack(HistVariable::MET, "background", true);
	THStack* hist2 = leptonBackgrounds->getStack(HistVariable::MET, "signal", true);
	TCanvas *cs = new TCanvas("cs", "cs", 10, 10, 700, 900);
	cs->SetWindowSize(1920, 1080);
	//Draw whichever hist has largest Y axis first
	hist2->Draw("HIST");
	hist1->Draw("HIST SAME");
	cs->Update();
	//cs->SaveAs("SuperPlot.png");
}