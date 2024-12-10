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

void calculatePunzi() {
    //Change extraText here
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary Data");
    //Change the filePath here. This should be the longest branch all input files have in common.
    //const std::string filePath = "/uscms/home/jpalamad/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/";
	const std::string fileName = "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/DataCollection/bin/BDT_CompleteCuts.root";

    //Change x and y axis titles here
    TString xTitle = "BDT Response";
    TString yTitle = "Punzi Criterion";

    TFile* openedFile;
    
    openedFile = TFile::Open(fileName.c_str(), "read");
    if(!openedFile) {
        throw std::runtime_error("Cannot open file!");
        }

    TCanvas *c = (TCanvas *)openedFile->Get("canvas1");
    TH1* sg_hist = (TH1*)c->GetPrimitive("MVA_BDT_S");
    TH1* bg_hist = (TH1*)c->GetPrimitive("MVA_BDT_B");

    if (!sg_hist)
    {
        throw std::runtime_error("Signal histogram not found!");
    } else if (!bg_hist)
    {
        throw std::runtime_error("Background histogram not found!");
    }

    TH1* punzi_hist = (TH1*)sg_hist->Clone("punzi_hist");
    punzi_hist->Reset();

    // No need to rebin - bdt sg and bg files already binned equally
	// for (int i = 0; i < count; i++)
	// {
	// 	histVector[i]->Rebin(10);
	// }

	// for (int i = 0; i < count; i++)
	// {
    //     histVector[i]->Add(histVector[count - 1], -1);
	// 	   histVector[i]->Scale(100.0);

    //     histVector[i]->Divide(histVector[count - 1]);

	// 	//histVector[i]->Add(fa);

	// 	TAxis* y = histVector[i]->GetYaxis();
	// 	histVector[i]->ExtendAxis(-800, y);
    // }

    // signal darphotonbaserun2
    // background dy50run2

    for(int j = 0; j < (sg_hist->GetNbinsX());j++)
    {
        double sgval = sg_hist->Integral(j, sg_hist->GetNbinsX());
        double bgval = bg_hist->Integral(j, bg_hist->GetNbinsX());
        // std::cout<<sgval<<"\n";
        // std::cout<<bgval<<"\n";
        double luminosity = 139000; //inverse picobarns
        double cross_section = 5735; // picobarns
        double punzi = sgval / std::sqrt(2.5 + bgval * cross_section * luminosity / bg_hist->GetEntries());
        std::cout << sg_hist->GetXaxis()->GetBinCenter(j) << "|" << sgval << "|" << bgval << "|" << punzi << "\n";
        punzi_hist->SetBinContent(j, punzi);
    }

    TCanvas *canvas = plotFormatter->simple1DHist(punzi_hist, xTitle, yTitle);
    canvas->SaveAs("bdt_qcd_punzi.png");
}

void bdtPunzi()
{
    calculatePunzi();
}