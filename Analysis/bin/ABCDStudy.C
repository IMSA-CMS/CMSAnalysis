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

void CalculateValues(std::vector<std::string> plotNames, std::vector<std::string> rowNames, const std::string inputAnalysisPath, const std::string crossSectionPath, const std::string channelName) {
    // Initialize DarkPhotonMCAnalysis with the input path and cross-sections file
    auto analysis = std::make_shared<DarkPhotonMCAnalysis>(inputAnalysisPath, crossSectionPath);

    // Get all channels and check if channels are available
    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();
    if (channels.empty()) {
        std::cerr << "Error: No channels found in the analysis." << std::endl;
        return;
    }

    // Label signal and background processes
    for (auto& channel : channels) {
        for (const std::string& processName : channel->getNames()) {
            if (processName == "signal") {
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

    std::vector<std::tuple<std::string, std::string, double, double>> results;

    for (std::string p : plotNames) {
        const char* plotName = p.c_str();

        std::vector<std::vector<TH1*>> hists;

        // Retrieve histograms for each background and region
        for (auto backgroundName : backgroundNames) {
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

            // Perform calculations on the histograms

            if (backgroundHists.size() < 4) {
                std::cerr << "Error: Incomplete histogram set for background " << backgroundName << std::endl;
                continue;
            }

            // Retrieve individual histograms
            TH1* hmss = backgroundHists[0];
            TH1* lmss = backgroundHists[1];
            TH1* hmos = backgroundHists[2];
            TH1* lmos = backgroundHists[3];

            if (!hmss || !lmss || !hmos || !lmos) {
                std::cerr << "Error: One or more histograms are missing for background " << backgroundName << std::endl;
                continue;
            }

            // Check for non-zero integral to avoid division by zero
            Double_t f_hmss = hmss->Integral();
            Double_t f_lmss = lmss->Integral();
            Double_t f_hmos = hmos->Integral();
            Double_t f_lmos = lmos->Integral();

            double calculated = (f_hmss != 0) ? (f_lmss * f_hmos) / f_hmss : 0.0;
            
            results.push_back(std::make_tuple(p, backgroundName, f_lmos, calculated));
        }
    }

    // Print stored results at the end

    std::string lastPrintedPlot = "";

    for (const auto& result : results) {
        if (std::get<0>(result) != lastPrintedPlot) {
            std::cout << "########################### " << std::get<0>(result) << " ###########################\n";
            lastPrintedPlot = std::get<0>(result);
        }

        std::cout << "-------------------- " << std::get<1>(result) << " --------------------" << std::endl;
        std::cout << "Expected (Low Mass, Different Sign): " << std::get<2>(result) << std::endl;
        if (std::get<2>(result) != 0) {
            std::cout << "Calculated (ABCD Prediction): " << std::get<3>(result) << std::endl;
        } else {
            std::cerr << "Warning: Division by zero for High Mass, Same Sign in background " << std::get<1>(result) << std::endl;
        }
    }
}

void ABCDStudy() {
    const std::string crossSectionPath = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections.txt";
    const std::string inputAnalysisPath = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/LeptonJetReconstruction_All/";
    // const std::string inputAnalysisPath = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Output/";
    
    const std::string channelName = "0.3";

    // no point in using multiple variables - integral is the same
    // std::vector<std::string> columnNames = {"Eta", "Lepton Jet Delta R", "Lepton Jet Mass", "Phi", "Pt"};
    std::vector<std::string> columnNames = {"Pt"};
    std::vector<std::string> regionNames = {"High Mass and Same Sign", "Low Mass and Same Sign", "High Mass and Different Sign", "Low Mass and Different Sign"};

    CalculateValues(columnNames, regionNames, inputAnalysisPath, crossSectionPath, channelName);
}
