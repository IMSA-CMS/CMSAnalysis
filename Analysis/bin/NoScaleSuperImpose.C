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

void NoScaleSuperImpose() {
    //Change extraText here
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary");
    //Change the filePath here. This should be the longest branch all input files have in common.
    //const std::string filePath = "/uscms/home/aytang/RecoWidth/CMSSW_11_0_2/src/CMSAnalysis/DataCollection/bin/";
    const std::string filePath = "/uscms/home/vyou/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/Muon/";
    //Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
    //std::vector<std::string> files = {"Delta RAcceptance1000.root", "Delta RAcceptance1000.root"};
    std::vector<std::string> files = {"Higgs1000Run2.root", "Data_Trigger_SingleMuon_Year_2017C.root", "Drell-Yan_MassCut_10-50_Run_2.root"};
    std::vector<std::string> hists = {"uuuu_uuuu__hists/uuuu_uuuu_1st Highest mu- Dz", "uuuu__hists/uuuu_1st Highest mu- Dz", "uuuu__hists/uuuu_1st Highest mu- Dz"};
    //std::vector<std::string> hists = {"Cut4MET", "Cut4MET", "Cut4MET"};
    //Write your graph names here (for the legend)
    std::vector<TString> names = {"Higgs1000Run2Iso", "Data2017CIso", "DY1050Iso"};
    //Colors go here
    std::vector<int> colors = {1, 3, 5};
    //Change x and y axis titles here
    //TString xTitle = "Delta R";
    TString xTitle = "Invariant Mass (GeV)";
    TString yTitle = "Number of Events"; 

    int count = 0;
    TFile* openedFile;
    TH1* hist;
    std::vector<TH1*> histVector;
    std::string fileName;
    for(std::string file : files) {
	    fileName = filePath + file; 
        std::cout << fileName << std::endl;
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
    
    TCanvas *canvas = plotFormatter->noScaleSimpleSuperImposedHist(histVector, colors,  names, xTitle, yTitle);
}
