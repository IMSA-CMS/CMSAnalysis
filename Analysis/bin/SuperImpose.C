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

    //Change the filePath here. This should be the longest branch all input files have in common. CHANGE USERNAME TO YOUR USERNAME
    const std::string filePath = "/uscms/home/ceddingt/analysis/CMSSW_14_0_4/src/CMSAnalysis/";

    //Pick source files for histograms here
    std::vector<std::string> files = {"testpdfbackground.root"}; 

    //Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
    std::vector<std::string> hists = {"DefaultHistogram", "LowHistogram", "HighHistogram"};
    //Write your graph names here (for the legend)
    std::vector<TString> names = {"Default", "Low Weight", "High Weight"};
    

    //Colors go here
    std::vector<int> colors = {1, 2, 3, 4, 5};
    //Change x and y axis titles here
    TString xTitle = "Same-sign Dilepton Mass (GeV)";
    TString yTitle = "Number of Events";


    int count = 0;
    TFile* openedFile;
    TH1* hist;
    std::vector<TH1*> histVector;
    std::string fileName;
    for(std::string file : files) {
	    fileName = filePath + file; 
        openedFile = TFile::Open(fileName.c_str(), "read");
        if(!openedFile) 
        {
            throw std::runtime_error("Cannot open file!");
        }
        for(int i = 0; i < hists.size(); i++){

            hist = dynamic_cast<TH1*>(openedFile->Get(hists.at(i).c_str()));
            if (!hist)
            {
                throw std::runtime_error("Histogram " + hists.at(i) + " not found!");
            }
            if(dynamic_cast<TH2 *>(hist) != 0) {
                TH2* hist2D = dynamic_cast<TH2 *>(hist);
                TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
                histVector.push_back(newhist);
            }
            else {
                histVector.push_back(hist);
            }
        }
    }
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "");
    std::cout << "SuperImpose Finished Iteration " << histVector.size() <<std::endl;
    TCanvas *canvas = plotFormatter->simpleSuperImposedHist(histVector, colors, names, xTitle, yTitle);

    //find integral of the different histograms
    for(int i = 0; i < histVector.size(); i++) {
        std::cout << "Integral of " << names.at(i) << " is " << histVector.at(i)->Integral() << std::endl;
    }
    
}
