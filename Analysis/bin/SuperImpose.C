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

void SuperImpose() {
    //Change extraText here
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary");
    //Change the filePath here. This should be the longest branch all input files have in common.
    //const std::string filePath = "/uscms/home/aytang/RecoWidth/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
    const std::string filePath = "/uscms/home/vrao/analysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/";
    //Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
    std::vector<std::string> files = {"GammaAcceptance1000.root", "GammaAcceptance1000.root"};
    //std::vector<std::string> files = {"HiggsRunCuts/Higgs700.root", "BackgroundRunCuts/TTBarPick500.root", "BackgroundRunCuts/ZZPick4.root"};
    std::vector<std::string> hists = {"Gen Sim Gamma (Reconstructed 1 Jets)", "Gen Sim Gamma (Reconstructed 2 Jets)"};
    //std::vector<std::string> hists = {"Cut4MET", "Cut4MET", "Cut4MET"};
    //Write your graph names here (for the legend)
    std::vector<TString> names = {"1 Jet", "2 Jets"};
    //Colors go here
    std::vector<int> colors = {1, 2};
    //Change x and y axis titles here
    TString xTitle = "Gamma";
    TString yTitle = "Events (1/Integral)";


    int count = 0;
    TFile* openedFile;
    TH1* hist;
    std::vector<TH1*> histVector;
    std::string fileName;
    for(std::string file : files) {
	    fileName = filePath + file; 
        openedFile = TFile::Open(fileName.c_str(), "read");
        if(!openedFile) {
            throw std::runtime_error("Cannot open file!");
            }
        hist = dynamic_cast<TH1*>(openedFile->Get(hists.at(count).c_str()));
        if(dynamic_cast<TH2 *>(hist) != 0) {
            TH2* hist2D = dynamic_cast<TH2 *>(hist);
            TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
            histVector.push_back(newhist);
        }
        else {
            histVector.push_back(hist);
        }
        count++;
    }
    
    TCanvas *canvas = plotFormatter->simpleSuperImposedHist(histVector, colors,  names, xTitle, yTitle);
}
