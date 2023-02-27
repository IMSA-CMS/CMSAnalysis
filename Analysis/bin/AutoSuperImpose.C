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

void AutoSuperImpose() {
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary");
    //Folder path for your files 
    const std::string filePath = "~/";
    const std::string outputFile = "AutoSuperImpose_Higgsto2ZDFermion.root";
    //List of files to superimpose
    std::vector<std::string> files = {"nobackup/rootfiles/Higgsto2ZD/Hist_Higgsto2ZD_300.root", "CMSAnalysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/Hist_Higgsto2ZD_Fermion100.root", "CMSAnalysis/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/Hist_Higgsto2ZD_Fermion150.root"};   
    //List of histograms to be fetched in ever file
    std::vector<std::string> histnames = {"1st Highest Dark Photon Pt","1st Highest Dark Photon Daughter Delta R","Delta R"};
    //Labels for the legend    
    std::vector<TString> names = {"Higgsto2ZD", "Fermion100","Fermion150"};
    //Colors go here
    std::vector<int> colors = {1, 2, 3, 4, 5};
    TString yTitle = "Events (1/Integral)";

    TFile* openedFile;
    TH1* hist;
    std::vector<std::vector<TH1*>> histVectors;
    std::string fileName;
    for (auto histname : histnames)
    {
        std::vector<TH1*> histVector;
        for(std::string file : files) 
        {
	        fileName = filePath + file; 
            openedFile = TFile::Open(fileName.c_str(), "read");
            if(!openedFile) 
            {
                throw std::runtime_error("Cannot open file!");
            }  
            hist = dynamic_cast<TH1*>(openedFile->Get(histname.c_str()));
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
            if(!hist)
            {
                throw std::runtime_error("Cannot find hist: \"" + histname +"\"");
            }
        }
        histVectors.push_back(histVector);
    }

    TFile *outputRootFile = new TFile(outputFile.c_str(), "RECREATE");
    outputRootFile->cd();
    int count = 0;
    for (auto histVector : histVectors)
    {
        TString xTitle = histVector.at(0)->GetXaxis()->GetTitle();
        TCanvas *canvas = plotFormatter->simpleSuperImposedHist(histVector, colors, names, xTitle, yTitle);
        canvas->Write(histnames.at(count).c_str());
        count++;
    }
    outputRootFile->Close();
    delete outputRootFile;
}
