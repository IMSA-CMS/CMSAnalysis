#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/DarkPhotonMCAnalysis.hh"
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

void ABCDStudy(const char* plotName) {
    const std::string channelName = "0.3";
    const std::string inputAnalysisPath = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Output/";

    // Initialize DarkPhotonMCAnalysis with the input path and cross-sections file
    auto analysis = std::make_shared<DarkPhotonMCAnalysis>(inputAnalysisPath, "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections_config1.txt");

    // Get all channels and check if channels are available
    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();
    if (channels.empty()) {
        std::cerr << "Error: No channels found in the analysis." << std::endl;
        return;
    }

    // Label signal and background processes
    for (auto& channel : channels) {
        for (const std::string& processName : channel->getNames()) {
            if (processName == "Dark Photon Signal") {
                channel->labelProcess("signal", processName);
            } else {
                channel->labelProcess("bg", processName);
            }
        }
    }

    // Retrieve background processes
    std::vector<std::string> backgroundNames = channels.at(0)->getNamesWithLabel("bg");
    if (backgroundNames.empty()) {
        std::cerr << "Error: No background processes found." << std::endl;
        return;
    }

    std::vector<std::vector<TH1*>> hists;
    std::vector<std::string> rowNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Sign", "Low Mass and Different Sign"};

    // Retrieve histograms for each background and region
    for (const auto& backgroundName : backgroundNames) {
        std::vector<TH1*> backgroundHists;

        for (const auto& region : rowNames) {
            std::string varName = std::string(plotName) + " " + region;
            HistVariable histvariable(varName);

            TH1* hist = analysis->getHist(histvariable, backgroundName, false, channelName);
            if (hist) {
                backgroundHists.push_back(hist);
            } else {
                std::cerr << "Warning: Histogram for " << varName << " and background " << backgroundName << " not found." << std::endl;
                backgroundHists.push_back(nullptr); // Add a placeholder for missing histogram
            }
        }

        hists.push_back(backgroundHists);
    }

    // Perform calculations on the histograms
    for (size_t i = 0; i < hists.size(); ++i) {
        const auto& backgroundHists = hists[i];
        if (backgroundHists.size() < 4) {
            std::cerr << "Error: Incomplete histogram set for background " << backgroundNames[i] << std::endl;
            continue;
        }

        // Retrieve individual histograms
        TH1* hmss = backgroundHists[0];
        TH1* lmss = backgroundHists[1];
        TH1* hmos = backgroundHists[2];
        TH1* lmos = backgroundHists[3];

        if (!hmss || !lmss || !hmos || !lmos) {
            std::cerr << "Error: One or more histograms are missing for background " << backgroundNames[i] << std::endl;
            continue;
        }

        // Check for non-zero integral to avoid division by zero
        Double_t f_hmss = hmss->Integral();
        Double_t f_lmss = lmss->Integral();
        Double_t f_hmos = hmos->Integral();
        Double_t f_lmos = lmos->Integral();

        std::cout << "-------------------- " << backgroundNames[i] << " --------------------" << std::endl;
        std::cout << "Expected (Low Mass, Different Sign): " << f_lmos << std::endl;
        if (f_hmss != 0) {
            std::cout << "Calculated (ABCD Prediction): " << (f_lmss * f_hmos) / f_hmss << std::endl;
        } else {
            std::cerr << "Warning: Division by zero for High Mass, Same Sign in background " << backgroundNames[i] << std::endl;
        }
    }
}
