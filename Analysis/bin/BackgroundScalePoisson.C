#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/DarkPhotonCompleteAnalysis.hh"
#include <memory>
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TFile.h"
#include "TH1.h"
#include "TMath.h"
#include <iostream>
#include <vector>

// Global variables to be used in the log-likelihood function
TH1* globalData;
TH1* globalDynamicBG;
TH1* globalFixedBG;

// Poisson log-likelihood calculation function for grid scan
double calculatePoissonLogLikelihood(double scaleDynamic, double scaleFixed) {
    TH1* scaledDynamicBG = (TH1*)globalDynamicBG->Clone();
    scaledDynamicBG->Scale(scaleDynamic);

    TH1* scaledFixedBG = (TH1*)globalFixedBG->Clone();
    scaledFixedBG->Scale(scaleFixed);

    TH1* totalBG = (TH1*)scaledDynamicBG->Clone();
    totalBG->Add(scaledFixedBG);

    double logLikelihood = 0.0;
    for (int i = 1; i <= globalData->GetNbinsX(); ++i) {
        double observed = globalData->GetBinContent(i);
        double expected = totalBG->GetBinContent(i);
        if (expected > 0) {
            logLikelihood -= observed * TMath::Log(expected) - expected;
        }
    }

    delete scaledDynamicBG;
    delete scaledFixedBG;
    delete totalBG;

    return logLikelihood;
}

void fitAndDisplayHistograms(const char* plotName, const char* outputName) {
    const std::string dynamicBGName = "QCD Background";
    const std::string channelName = "0.3";
    const std::string inputAnalysisPath = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Output/";

    auto analysis = std::make_shared<DarkPhotonCompleteAnalysis>(inputAnalysisPath, "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections_config1.txt");

    // Get channels and background histograms
    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();
    std::shared_ptr<Channel> channel = channels.at(0);

    for (std::shared_ptr<Channel> channel : channels) {
        for (std::string processName : channel->getNames()) {
            if (processName == "Dark Photon Signal") {
                channel->labelProcess("signal", processName);
            } else if (processName == "Data") {
                channel->labelProcess("data", processName);
            } else if (processName == dynamicBGName) {
                channel->labelProcess("dynamicbg", processName);
            } else {
                channel->labelProcess("fixedbg", processName);
            }
        }
    }

    std::vector<std::string> backgroundNames = channel->getNamesWithLabel("fixedbg");
    std::vector<std::string> dataNames = channel->getNamesWithLabel("data");

    // Retrieve histograms
    HistVariable histvariable(plotName, "BackgroundScale");
    globalData = analysis->getHist(histvariable, dataNames.at(0), false, channelName);
    globalDynamicBG = analysis->getHist(histvariable, dynamicBGName, false, channelName);

    // Create collective fixed background histogram
    globalFixedBG = (TH1*)globalDynamicBG->Clone();
    globalFixedBG->Reset();
    for (const std::string& name : backgroundNames) {
        TH1* fixedHist = analysis->getHist(histvariable, name, true, channelName);
        globalFixedBG->Add(fixedHist);
    }

    // Draw initial stacked histogram
    TCanvas* canvas1 = new TCanvas("InitialStack", "Initial Stacked Histogram", 800, 600);
    THStack* stack = new THStack("stack", "Initial Background Components;Mass;Events");

    globalDynamicBG->SetFillColor(kRed);
    globalFixedBG->SetFillColor(kBlue);
    globalData->SetMarkerStyle(20);

    stack->Add(globalFixedBG);
    stack->Add(globalDynamicBG);

    stack->Draw("HIST");
    globalData->Draw("E SAME");

    TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(globalData, "Data", "lep");
    legend->AddEntry(globalFixedBG, "Fixed Background", "f");
    legend->AddEntry(globalDynamicBG, "Dynamic Background", "f");
    legend->Draw();

    canvas1->SaveAs("InitialStackedHistogram.root");
    canvas1->SaveAs("InitialStackedHistogram.png");

    // Grid scan parameters
    double minScaleDynamic = 0.01;
    double maxScaleDynamic = 0.7;
    double stepSizeDynamic = 0.01;

    double minScaleFixed = 0.001;
    double maxScaleFixed = 0.02;
    double stepSizeFixed = 0.001;

    // Variables to store best-fit results
    double bestScaleDynamic = 0.01;
    double bestScaleFixed = 0.01;
    double minLogLikelihood = 1e9;

    // Perform grid scan
    for (double scaleDynamic = minScaleDynamic; scaleDynamic <= maxScaleDynamic; scaleDynamic += stepSizeDynamic) {
        for (double scaleFixed = minScaleFixed; scaleFixed <= maxScaleFixed; scaleFixed += stepSizeFixed) {
            double logLikelihood = calculatePoissonLogLikelihood(scaleDynamic, scaleFixed);
            if (logLikelihood < minLogLikelihood) {
                minLogLikelihood = logLikelihood;
                bestScaleDynamic = scaleDynamic;
                bestScaleFixed = scaleFixed;
            }
        }
    }

    std::cout << "Best fit scale factors: " << std::endl;
    std::cout << "Dynamic Background Scale: " << bestScaleDynamic << std::endl;
    std::cout << "Fixed Background Scale: " << bestScaleFixed << std::endl;
    std::cout << "Log-Likelihood: " << minLogLikelihood << std::endl;

    std::cout << "-------------------------" << std::endl;
    std::cout << "Dynamic events: " << bestScaleDynamic * globalDynamicBG->GetEntries() << std::endl;
    std::cout << "Fixed events: " << bestScaleFixed * globalFixedBG->GetEntries() << std::endl;
    std::cout << "-------------------------" << std::endl;

    // Create best-fit stacked histogram
    TCanvas* canvas2 = new TCanvas("BestFitStack", "Best Fit Stacked Histogram", 800, 600);
    THStack* bestFitStack = new THStack("bestFitStack", "Best Fit Background Components;Mass;Events");

    TH1* bestFitDynamicBG = (TH1*)globalDynamicBG->Clone();
    bestFitDynamicBG->Scale(bestScaleDynamic);
    bestFitDynamicBG->SetFillColor(kRed);

    TH1* bestFitFixedBG = (TH1*)globalFixedBG->Clone();
    bestFitFixedBG->Scale(bestScaleFixed);
    bestFitFixedBG->SetFillColor(kBlue);

    bestFitStack->Add(bestFitFixedBG);
    bestFitStack->Add(bestFitDynamicBG);

    bestFitStack->Draw("HIST");
    globalData->Draw("E SAME");

    TLegend* bestFitLegend = new TLegend(0.7, 0.7, 0.9, 0.9);
    bestFitLegend->AddEntry(globalData, "Data", "lep");
    bestFitLegend->AddEntry(bestFitFixedBG, "Fixed Background (Best Fit)", "f");
    bestFitLegend->AddEntry(bestFitDynamicBG, "Dynamic Background (Best Fit)", "f");
    bestFitLegend->Draw();

    canvas2->SaveAs(outputName);
}

void BackgroundScalePoisson(const char* plotName, const char* outputName) {
    fitAndDisplayHistograms(plotName, outputName);
}