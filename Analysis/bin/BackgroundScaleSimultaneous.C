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
        return;
    }

    RooRealVar xAxis("xAxis", "xAxis", xMin, xMax);
    RooRealVar scalar("scalar", "scalar", 1, 0, maxScalar);

    RooPlot* frame = xAxis.frame();

    RooDataHist* scaleData = new RooDataHist("scaleData", "scaleData", xAxis, scaleHist);
    RooDataHist fitData("fitData", "fitData", xAxis, fitHist);

    RooHistPdf curve("curve", "curve", xAxis, xAxis, *scaleData, 0);
    RooExtendPdf scaledCurve("scaledCurve", "scaledCurve", curve, scalar);

    fitData.plotOn(frame);
    scaledCurve.fitTo(fitData);
    scaledCurve.plotOn(frame);
    curve.plotOn(frame);

    std::cout << "/////////////////////////////////////////\nScalar Value:" << std::endl;
    scalar.printValue(std::cout);
    std::cout << "\n/////////////////////////////////////////" << std::endl;

    frame->Draw();
}

void BackgroundScaleSimultaneous() {
    const std::vector<std::string> plotNames = {"Lepton Jet Mass Low Mass and Same Sign", 
                                                "Lepton Jet Mass High Mass and Same Sign", 
                                                "Lepton Jet Mass High Mass and Different Sign"};
    const std::string dynamicBGName = "QCD Background"; // process name for dynamic bg
    const std::string channelName = "0.3";
    const std::string inputAnalysisPath = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Output/";

    auto analysis = std::make_shared<DarkPhotonCompleteAnalysis>(inputAnalysisPath, "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/crossSections_config1.txt");

    std::vector<std::shared_ptr<Channel>> channels = analysis->getChannels();

    for (std::shared_ptr<Channel> channel : channels) {
        for (std::string processName : channel->getNames()) {
            if (processName == "Dark Photon Signal") {
                channel->labelProcess("signal", processName);
            }
            else if (processName == "Data") {
                channel->labelProcess("data", processName);
            }
            else if (processName == dynamicBGName) {
                channel->labelProcess("dynamicbg", processName);
            }
            else {
                channel->labelProcess("fixedbg", processName);
            }
        }
    }

    TH1* combinedData = nullptr;
    TH1* combinedDynamicBG = nullptr;
    std::vector<TH1*> combinedBackgroundHists;

    for (const std::string& plotName : plotNames) {
        auto histvariable = HistVariable(plotName, "BackgroundScale");

        std::shared_ptr<Channel> processes = channels.at(0);
        std::vector<std::string> backgroundNames = processes->getNamesWithLabel("fixedbg");
        std::vector<std::string> dataNames = processes->getNamesWithLabel("data");

        TH1* dynamicBG = analysis->getHist(histvariable, dynamicBGName, false, channelName);
        TH1* data = analysis->getHist(histvariable, dataNames.at(0), false, channelName);

        if (!combinedData) {
            combinedData = (TH1*)data->Clone("combinedData");
            combinedDynamicBG = (TH1*)dynamicBG->Clone("combinedDynamicBG");
        }
        else {
            combinedData->Add(data);
            combinedDynamicBG->Add(dynamicBG);
        }

        for (std::string name : backgroundNames) {
            TH1* backgroundHist = analysis->getHist(histvariable, name, true, channelName);
            if (combinedBackgroundHists.size() < backgroundNames.size()) {
                combinedBackgroundHists.push_back((TH1*)backgroundHist->Clone());
            }
            else {
                combinedBackgroundHists[&name - &backgroundNames[0]]->Add(backgroundHist);
            }
        }
    }

    TH1* fitHist = (TH1*)combinedData->Clone("fitHist");
    for (auto fixedHist : combinedBackgroundHists) {
        fitHist->Add(fixedHist, -1);
    }

    std::cout << "Number of bins in dynamic BG: " << combinedDynamicBG->GetNbinsX() << std::endl;
    std::cout << "Number of bins in fit histogram: " << fitHist->GetNbinsX() << std::endl;

    fitHistogram(combinedDynamicBG, fitHist, 0, 10, 100000);
}
