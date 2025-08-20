#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
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
#include <fmt/core.h>
#include "CMSAnalysis/Analysis/interface/DarkPhotonCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/src/DarkPhotonCompleteAnalysis.cc"

void createPunzi(const std::string& fileName, const std::string& signalHistName, const std::string& backgroundHistName, 
                    const TString& xTitle, const TString& yTitle, double luminosity, double cross_section, const std::string& outputFileName) {
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary Data");
	DarkPhotonCompleteAnalysis analysis; 
	auto channel = analysis.getChannel("0.3");
	auto signalHists = channel->getHists(HistVariable("LeptonJetMLOutput Low Mass and Same Sign"), "signal", true);
	auto backgroundHists = channel->getHists(HistVariable("LeptonJetMLOutput Low Mass and Same Sign"), "background", true);
	auto signalHist = Channel::combineHists(signalHists);
	auto backgroundHist = Channel::combineHists(backgroundHists);
	
	
    // TFile* openedFile = TFile::Open(fileName.c_str(), "read");
    // if (!openedFile) {
    //     throw std::runtime_error("Cannot open file: " + fileName);
    // }

    // TCanvas* c = (TCanvas*)openedFile->Get("canvas1");
    // if (!c) {
    //     throw std::runtime_error("Canvas 'canvas1' not found in file: " + fileName);
    // }

    // TH1* sg_hist = (TH1*)c->GetPrimitive(signalHistName.c_str());
    // TH1* bg_hist = (TH1*)c->GetPrimitive(backgroundHistName.c_str());

    // if (!sg_hist) {
    //     throw std::runtime_error("Signal histogram '" + signalHistName + "' not found!");
    // }
    // if (!bg_hist) {
    //     throw std::runtime_error("Background histogram '" + backgroundHistName + "' not found!");
    // }

    TH1* punziHist = (TH1*)signalHist->Clone("punziHist");
    punziHist->Reset();

    for (int j = 0; j < signalHist->GetNbinsX(); j++) {
        double sgval = signalHist->Integral(j, signalHist->GetNbinsX());
        double bgval = backgroundHist->Integral(j, backgroundHist->GetNbinsX());
        double punzi = sgval / std::sqrt(2.5 + bgval * cross_section * luminosity / backgroundHist->GetEntries());
        std::cout << signalHist->GetXaxis()->GetBinCenter(j) << "|" << sgval << "|" << bgval << "|" << punzi << "\n";
        punziHist->SetBinContent(j, punzi);
    }

    TCanvas* canvas = plotFormatter->simple1DHist(punziHist, xTitle, yTitle);
    canvas->SaveAs(outputFileName.c_str());


    //signal + background hist
    TCanvas* sigCanvas = plotFormatter->simple1DHist(signalHist, xTitle, "Signal Events");
    sigCanvas->SaveAs("signal_hist.png");
    TCanvas* bgCanvas = plotFormatter->simple1DHist(backgroundHist, xTitle, "Background Events");
    bgCanvas->SaveAs("background_hist.png");

}

void bdtPunziGeneral() {
    
    const std::string fileName = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/BDT_NewMin_Output.root"; // ROOT file path
    const std::string signalHistName = "MVA_BDT_S";            // signal histogram name
    const std::string backgroundHistName = "MVA_BDT_B";        // background histogram name
    const TString xTitle = "BDT Response";                     // x-axis title
    const TString yTitle = "Punzi Criterion";                  // y-axis title
    const double luminosity = 139000;                          // luminosity in inverse picobarns
    const double cross_section = 5735;                         // Cross-section in picobarns
    const std::string outputFileName = "generalized_punzi.png"; // Output name

    createPunzi(fileName, signalHistName, backgroundHistName, xTitle, yTitle, luminosity, cross_section, outputFileName);
}