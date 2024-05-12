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
	const std::string filePath = "/eos/uscms/store/user/jpalamad/output";
    //Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
    //Write your graph names here (for the legend)
    
    std::vector<std::string> files = {};
    std::vector<std::string> hists = {};
    std::vector<TString> names = {};

    //Colors go here
    std::vector<int> colors = {};

    const int base = 19;
    const std::string baseName = "NNPDF3.1 QCD+LUXQED NLO";
    const int run = 1;
    const std::vector<std::string> pdfNames = {
        "GRV 94L LO",
        "CTEQ 5L LO",
        "MRST LO",
        "MRST LO",
        "MSTW 2008 LO",
        "MSTW 2008 NLO",
        "CTEQ6L NLO",
        "CTEQ6L1 LO",
        "CTEQ66.00 NLO",
        "CT09MC1 LO",
        "CT09MCS NLO",
        "NNPDF2.3 QCD+QED LO"
    };

	for (int i = (run - 1) * 3 + 1; i <= run * 3; i++)
	{
		files.push_back(fmt::format("/output_10000_pSet{}.root", i));
		hists.push_back("Same Sign Invariant Mass");
		names.push_back(pdfNames[i-1]);
		colors.push_back(i + 3 - 3 * run);
	}

    files.push_back(fmt::format("/output_10000_pSet{}.root", base));
    hists.push_back("Same Sign Invariant Mass");
    names.push_back(baseName);
    colors.push_back(4);

    //Change x and y axis titles here
    TString xTitle = "Mass(GeV)";
    TString yTitle = "Events";

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
        hist = dynamic_cast<TH1*>(dir->Get(hists.at(count).c_str()));
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
            double baseValue = histVector[3]->GetBinContent(j);
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