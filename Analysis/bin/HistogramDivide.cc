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

void HistogramDivide() {
    //Change extraText here
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary");
    //Change the filePath here. This should be the longest branch all input files have in common.
     const std::string filePath = "/uscms/home/rmandell/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test";
    //Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
    //Write your graph names here (for the legend)
    
    std::vector<std::string> files = {"/PDF20Histograms.root", "/PDF21Histograms.root", "/PDF22Histograms.root", "/output_10000_pSet19.root"};
    std::vector<std::string> hists = {"Same Sign Invariant Mass", "Same Sign Invariant Mass", "Same Sign Invariant Mass", "Same Sign Invariant Mass"};
    std::vector<TString> names = {"NNPDF3.1 QCD+LUXQED NNLO(20)", "NNPDF3.1sx+LHCb NLO+NLLx LUXQED(21)", "NNPDF3.1sx+LHCb NNLO+NLLx LUXQED(22)", "NNPDF3.1 QCD+LUXQED NLO(19)"};

    //Colors go here
    std::vector<int> colors = {1, 2, 3, 4};
    //Change x and y axis titles here
    TString xTitle = "Mass(GeV)";
    TString yTitle = "Events";


    int count = 0;
    int base = 3;
    TFile* openedFile;
    TH1* hist;
    std::vector<TH1*> histVector;
    std::string fileName;
    for(std::string file : files) {
	    fileName = filePath + file; 
        if(count == 3){
            fileName = "/eos/uscms/store/user/jpalamad/output/output_10000_pSet19.root";
        }
        openedFile = TFile::Open(fileName.c_str(), "read");
        if(!openedFile) {
            throw std::runtime_error("Cannot open file!");
            }
        auto dir = openedFile->GetDirectory("_hists");
        hist = dynamic_cast<TH1*>(dir->Get(hists.at(count).c_str()));
        if (!hist)
        {
            throw std::runtime_error("Histogram " + hists.at(count) + " not found!");
        }
        if(dynamic_cast<TH2 *>(hist) != 0) {
            TH2* hist2D = dynamic_cast<TH2 *>(hist);
            TH1* newhist = hist2D->ProjectionX("_px", 0, -1, "E");
            histVector.push_back(newhist);
        }
        else {
            histVector.push_back(hist);
        }
        count++;
    }

    for (int i = 0; i < count; i++)
	{
		histVector[i]->Rebin(10);
	}

    double average[count];

    for (int i = 0; i < count; i++)
    {
        //histVector[i]->Add(histVector[base], -1);
        //histVector[i]->Divide(histVector[base]);
        for(int j = 0; j < (histVector[i]->GetNbinsX());j++)
        {
            double histValue = histVector[i]->GetBinContent(j);
            double baseValue = histVector[base]->GetBinContent(j);
            //std::cout<<baseValue<<"\n";
            //std::cout<<histValue<<"\n";
                
            if(baseValue == 0)
            {
                histVector[i]->SetBinContent(j, 0);
                average[i]+= 0;
            }else
            {
                double ratio = (histValue-baseValue) / baseValue;
                histVector[i]->SetBinContent(j, ratio);
                average[i]+= ratio;
            }
        }

        std::cout<<average[i]/histVector[i]->GetNbinsX() * 100<<"\n";
    }



    

   
   

    TCanvas *canvas = plotFormatter->noScaleSimpleSuperImposedHist(histVector, colors, names, xTitle, yTitle); 
    
}
