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
    const std::string filePath = "/uscms/home/cobriend/analysis/CMSSW_12_4_3/src/CMSAnalysis/Analysis/bin/";
    //Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
    //Write your graph names here (for the legend)

    // Run on eta, pT, mass, delta R

    // Plot of dXY, dZ without cuts for just background on a 2D plot
    
    //std::vector<std::string> files = {"regularBackground.root", "invertedBackground.root"};
    std::vector<std::string> files = {"regularSignal.root", "invertedSignal.root"};
    //std::vector<std::string> files = {"noCutSignal.root", "noCutBackground.root"};

    std::vector<std::string> hists = {"1st Highest Lepton Jet Pt", "1st Highest Lepton Jet Pt"};
    //std::vector<std::string> his.ts = {"1st Highest Lepton Jet Lepton Jet Mass Zoom", "1st Highest Lepton Jet Lepton Jet Mass Zoom"};
    //std::vector<std::string> hists = {"1st Highest Lepton Jet Lepton Jet Delta R", "1st Highest Lepton Jet Lepton Jet Delta R"};
    //std::vector<std::string> hists = {"1st Highest Lepton Jet Eta", "1st Highest Lepton Jet Eta"};
    //std::vector<std::string> hists = {"dXY dZ Hist", "dXY dZ Hist"};

    std::vector<TString> names = {"Regular", "Inverted"};
    //std::vector<TString> names = {"Signal", "Background"};

    //Colors go here
    std::vector<int> colors = {1, 2};
    //Change x and y axis titles here
    TString xTitle = "";
    TString yTitle = "";


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
            auto dir = openedFile->GetDirectory("_hists");
            if(!dir) {
            throw std::runtime_error("Cannot open directory!");
            }
        hist = dynamic_cast<TH1*>(dir->Get(hists.at(count).c_str()));
        if (!hist)
        {
            for (auto hist : hists)
            {
                std::cout << hist << std::endl;
            }
            throw std::runtime_error("Histogram " + hists.at(count) + " not found!");
        }
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
    TCanvas *canvas = plotFormatter->simpleSuperImposedHist(histVector, colors, names, xTitle, yTitle);
}
