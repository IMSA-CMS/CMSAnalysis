#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/DarkPhotonCompleteAnalysis.hh"
#include <fstream>
#include "THStack.h"
#include "TString.h"
#include <iostream>
#include <memory>	
#include "TCanvas.h"
#include "TSystem.h"
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TStyle.h"
#include "RooDataHist.h"
#include "RooArgList.h"
#include "RooRealVar.h"

void fitHistogram(TH1* scaleHist, TH1* fitHist, double xMin, double xMax, double maxScalar)
{
    if (scaleHist->GetNbinsX() != fitHist->GetNbinsX())
    {
        std::cout << "Your histograms don't have the same number of bins!" << std::endl;
    }

    RooRealVar xAxis("xAxis", "xAxis", xMin, xMax);
    RooRealVar scalar("scalar", "scalar", 1, 0, maxScalar);

    //RooRealVar constant("constant", "constant", 1, 1, 1);

    RooPlot* frame = xAxis.frame();

    RooDataHist* scaleData = new RooDataHist("scaleData", "scaleData", xAxis, scaleHist);
    RooDataHist fitData("fitData", "fitData", xAxis, fitHist);

    RooHistPdf curve("curve", "curve", xAxis, xAxis, *scaleData, 0);
    RooExtendPdf scaledCurve("scaledCurve", "scaledCurve", curve, scalar);
    //RooExtendPdf scaledCurve("scaledCurve", "scaledCurve", curve, constant);

    fitData.plotOn(frame);
    scaledCurve.fitTo(fitData);
    scaledCurve.plotOn(frame);
    curve.plotOn(frame);
    frame->Draw();
}

void BackgroundScale() {
    const std::string plotName = "Lepton Jet Mass Low Mass and Same Sign";
    const std::string dynamicBGName = "QCD Background"; // process name for dynamic bg
    const std::string channelName = "0.3";
    const std::string inputAnalysisPath = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Output/";

    auto analysis = std::make_shared<DarkPhotonCompleteAnalysis>(inputAnalysisPath, "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections_config1.txt"); // BDT Output Analysis (LeptonJetMLOutput)
	//auto analysis = std::make_shared<DarkPhotonInputAnalysis>(inputAnalysisPath); // Variable Isolation Plots

    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();

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
			} else if (processName == dynamicBGName) {
                channel->labelProcess("dynamicbg", processName);
            } else {
				channel->labelProcess("fixedbg", processName);
			}
		}
	}

    TH1* data; // complete data
    TH1* dynamicBG; // qcd
    std::vector<TH1*> backgroundHists; // fixedbg
    
    std::shared_ptr<Channel> processes = 0;

	auto histvariable = HistVariable(plotName, "BackgroundScale");

    processes = channels.at(0);
    std::vector<std::string> backgroundNames = processes->getNamesWithLabel("fixedbg");
    std::vector<std::string> dataNames = processes->getNamesWithLabel("data");

    dynamicBG = analysis->getHist(histvariable, dynamicBGName, false, channelName);

    data = analysis->getHist(histvariable, dataNames.at(0), false, channelName);

    for(std::string name : backgroundNames) {
        backgroundHists.push_back(analysis->getHist(histvariable, name, true, channelName));
    }

    TH1* fitHist(data);
    for (auto fixedHist : backgroundHists)
    {
        fitHist->Add(fixedHist, -1);
    }

    
    std::cout << "number of bins is: " << dynamicBG->GetNbinsX();
    std::cout << "number of bins is: " << fitHist->GetNbinsX();

    fitHistogram(dynamicBG, fitHist, 0, 10, 100000);
}