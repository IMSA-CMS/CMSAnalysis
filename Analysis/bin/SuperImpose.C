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
    
    // std::vector<std::string> files = {"test1000.root", "HiggsDPZ.root", "ZPrime.root", "SUSY.root", "Higgs4DP.root"};
    // std::vector<std::string> hists = {"Gamma Values", "Gamma Values", "Gamma Values", "Gamma Values", "Gamma Values"};
    // std::vector<TString> names = {"Higgs to 2 Dark Photon", "Higgs to Z and Dark Photon", "Z Prime", "SUSY", "Higgs to 4 Dark Photon"};
    std::vector<std::string> files = {"signTest.root", "signTest.root", "signTest.root"};
    std::vector<std::string> hists = {"Low Mass and Different Signs__hists/Low Mass and Different Signs_1st Highest Lepton Jet Lepton Jet Delta R",  
    "Low Mass and All Negative__hists/Low Mass and All Negative_1st Highest Lepton Jet Lepton Jet Delta R",  
    "Low Mass and All Positive__hists/Low Mass and All Positive_1st Highest Lepton Jet Lepton Jet Delta R"};
    std::vector<TString> names = {"Low Mass OS", "Low Mass -S", "Low Mass +S"};
    //Colors go here
    std::vector<int> colors = {1, 2, 3, 4, 6, 7};
    //Change x and y axis titles here
    //TString xTitle = "Gamma";
    TString xTitle = "Lepton Jet Delta R";
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
        std::string name = hists.at(count);
        uint pos = name.find("/");
        std::string folder = name.substr(0,pos);
		std::string histName = name.substr(pos+1);
        auto dir = openedFile->GetDirectory(folder.c_str());
        if(!dir) {
            throw std::runtime_error("Cannot find directory!");
            }
        hist = dynamic_cast<TH1*>(dir->Get(histName.c_str()));
        if (!hist)
        {
            throw std::runtime_error("Histogram " + histName + " not found!");
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
