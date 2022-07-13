#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/ChannelLoader.hh"
#include <fstream>
#include "THStack.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"


void SuperPlot()
{
	std::shared_ptr<Channel> leptonBackgrounds = ChannelLoader::makeChannel("default", 700);
	THStack* hist = leptonBackgrounds->getStack("", true);
	TCanvas *cs = new TCanvas("cs", "cs", 10, 10, 700, 900);
	cs->SetWindowSize(1920, 1080);
	hist->Draw("hist");
	cs->Update();
	//cs->SaveAs("SuperPlot.png");
}