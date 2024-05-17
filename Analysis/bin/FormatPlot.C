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

void FormatPlot()
{
    std::cout << "Hello!\n";
    //Change extraText here
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Simulation Preliminary");
    //Change the filePath here. This should be the longest branch all input files have in common.
    //const std::string filePath = "/uscms/home/mkubon/analysis/clean/CMSSW_12_4_3/src/CMSAnalysis/";
    const std::string filePath = "";
    //Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
    //Write your graph names here (for the legend)

    std::string file = "darkPhoton0_3.root";
    std::string histName = "_hists/Zoomed In Lepton Jet Invariant Mass";

    TString xTitle = "Lepton Jet Invariant Mass";
    TString yTitle = "Events";

    TFile* openedFile;
    TH1* hist;
    std::vector<TH1*> histVector;
    std::string fileName;

    fileName = filePath + file; 
    openedFile = TFile::Open(fileName.c_str(), "read");
    if(!openedFile) 
    {
        throw std::runtime_error("Cannot open file!");
    }
    uint pos = histName.find("/");
    std::string folder = histName.substr(0,pos);
    histName = histName.substr(pos+1);
    auto dir = openedFile->GetDirectory(folder.c_str());
    if(!dir)
    {
        throw std::runtime_error("Cannot find directory!");
    }
    hist = dynamic_cast<TH1*>(dir->Get(histName.c_str()));
    if (!hist)
    {
        throw std::runtime_error("Histogram " + histName + " not found!");
    }
    if(dynamic_cast<TH2 *>(hist) != 0)
    {
        TH2* hist2D = dynamic_cast<TH2 *>(hist);
        TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
        histVector.push_back(newhist);
    }
    else
    {
        histVector.push_back(hist);
    }
  

    TCanvas *canvas = plotFormatter->simple1DHist(histVector[0], xTitle, yTitle);
}
