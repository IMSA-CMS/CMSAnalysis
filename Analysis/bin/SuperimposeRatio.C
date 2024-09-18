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

void RunSuperimpose() {
        //Change extraText here
    auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary Data");
    //Change the filePath here. This should be the longest branch all input files have in common.
    //const std::string filePath = "/uscms/home/jpalamad/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/";
	const std::string filePath = "/uscms/home/sdulam/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/";
    //Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
    //Write your graph names here (for the legend)
    std::vector<std::string> files = {"mctestdown.root", "mctestup.root", "mctestnorm.root"};
    std::vector<std::string> hists = {"e e__hists/e e_e- Opposite Sign Invariant Mass", "e e__hists/e e_e- Opposite Sign Invariant Mass", "e e__hists/e e_e- Opposite Sign Invariant Mass"};

    std::vector<TString> names = {"Electron Energy ScaleDown Ratio", "Electron Energy ScaleUp Ratio", "Electron Energy NoScale Ratio (0)"};

    //Colors go here
    std::vector<int> colors = {1,2,3};



    //Change x and y axis titles here
    TString xTitle = "Invariant Mass (GeV)";
    TString yTitle = "Event Ratio";

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
        if (!hist)
        {
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
 
	for (int i = 0; i < count; i++)
	{
		histVector[i]->Rebin(10);
	}

	// for (int i = 0; i < count; i++)
	// {
    //     histVector[i]->Add(histVector[count - 1], -1);
	// 	   histVector[i]->Scale(100.0);

    //     histVector[i]->Divide(histVector[count - 1]);

	// 	//histVector[i]->Add(fa);

	// 	TAxis* y = histVector[i]->GetYaxis();
	// 	histVector[i]->ExtendAxis(-800, y);
    // }

    double average[count];

    for (int i = 0; i < count; i++)
    {
        //histVector[i]->Add(histVector[base], -1);
        //histVector[i]->Divide(histVector[base]);
        for(int j = 0; j < (histVector[i]->GetNbinsX());j++)
        {
            double histValue = histVector[i]->GetBinContent(j);
            double baseValue = histVector[2]->GetBinContent(j);
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

void SuperimposeRatio() {
    int x = 3;
    int y = 4;

    RunSuperimpose();
}