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

double calculatePunzi(const std::string fileName) {
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary Data");
    //const std::string fileName = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/TMVAOutputs/TMVA_sgUniform_bfUniform_0_1_1_1.root";

    TString xTitle = "BDT Response";
    TString yTitle = "Punzi Criterion";

    TFile* openedFile = TFile::Open(fileName.c_str(), "read");
    if (!openedFile) {
        throw std::runtime_error("Cannot open file!");
    }

    TH1* sg_hist = (TH1*)openedFile->Get("dataset/Method_BDT/BDT/MVA_BDT_S");
    TH1* bg_hist = (TH1*)openedFile->Get("dataset/Method_BDT/BDT/MVA_BDT_B");

    if (!sg_hist) {
        throw std::runtime_error("Signal histogram not found!");
    }
    if (!bg_hist) {
        throw std::runtime_error("Background histogram not found!");
    }

    TH1* punzi_hist = (TH1*)sg_hist->Clone("punzi_hist");
    punzi_hist->Reset();

    double maxPunzi = -1.0;
    double maxPunziBinLeftEdge = 0.0;


    for (int j = 0; j < (sg_hist->GetNbinsX()); j++) {
        double sgval = sg_hist->Integral(j, sg_hist->GetNbinsX());
        double bgval = bg_hist->Integral(j, bg_hist->GetNbinsX());
        double luminosity = 139000; // inverse picobarns
        double cross_section = 5735; // picobarns
        double punzi = sgval / std::sqrt(2.5 + bgval * cross_section * luminosity / bg_hist->GetEntries());

        if (punzi > maxPunzi) {
            maxPunzi = punzi;
            maxPunziBinLeftEdge = sg_hist->GetXaxis()->GetBinLowEdge(j);
        }

        punzi_hist->SetBinContent(j, punzi);
    }

    std::string histTitle = fileName.substr(fileName.find("TMVA_"), fileName.find(".root")).c_str();
    TCanvas *canvas = plotFormatter->simple1DHist(punzi_hist, xTitle, yTitle);
    canvas->SaveAs(("PunziHists/CLIPunzi_" + histTitle).c_str());

    // Return the x-value corresponding to the maximum Punzi value
    std::cout << maxPunziBinLeftEdge << std::endl;

    return maxPunziBinLeftEdge;
}

void cliPunzi(const std::string fileName) {
    calculatePunzi(fileName);
}