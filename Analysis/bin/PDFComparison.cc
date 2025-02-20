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

void PDFComparison()
{
	auto plotFormatter = std::make_shared<PlotFormatter>(false, "Preliminary");
    //Change the filePath here. This should be the longest branch all input files have in common.
    const std::string filePath = "/uscms/home/rmandell/nobackup/5000Hists/events_5000_Hist";
  	//First order is to read root files to obtain values
    std::string histType = "Same Sign Invariant Mass";
	std::vector<std::string> hists = {"Same Sign Invariant Mass"};
    std::vector<TString> names = {"-1#sigma", "+1#sigma", "Baseline"};

    //Colors go here
    std::vector<int> colors = {4, 2, 3};
    //Change x and y axis titles here
    TString xTitle = "Mass(GeV)";
    TString yTitle = "Events";


    int count = 0;
    int base = 100;
    TFile* openedFile;
    TH1* hist;
    std::vector<TH1*> histVector;
    std::string fileName;
	int amountRun = 100;
    for(int i = 1; i < amountRun; i++) {
        if(i < 10){
            fileName = filePath + "0" + i + ".root";
        } 
        else if(i == 50 || i== 84){
            i++;
            fileName = filePath + i + ".root";
        }
        else{
            fileName = filePath + i + ".root"; 
        }
	    
        //Need to generate baseline funtion
        if(count == amountRun - 1){
            fileName = filePath + "baseline.root";
        }
        std::cout<<i<<"\n";
        openedFile = TFile::Open(fileName.c_str(), "read");
        if(!openedFile) {
            throw std::runtime_error("Cannot open file!");
            }
        auto dir = openedFile->GetDirectory("_hists");
        hist = dynamic_cast<TH1*>(dir->Get(hists.at(0).c_str()));
        if (!hist)
        {
            throw std::runtime_error("Histogram not found!");
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

	for (int i = 0; i < histVector.size(); i++)
	{
		histVector[i]->Rebin(10);
	}



	std::vector<double> allValues;

    for (int i = 0; i < histVector[0]->GetNbinsX(); i++)
    {
		//Count - 1 because we don't want to include baseline
        for(int j = 0; j < count - 1;j++)
        {
            allValues.push_back(histVector[j]->GetBinContent(i));
        }
		std::sort(allValues.begin(), allValues.end()); 
		double lowValue = allValues.at((int)(allValues.size()/100.0 * 16));
		double highValue = allValues.at((int)(allValues.size()/100.0 * 82));
		histVector[0]->SetBinContent(i, lowValue);
		histVector[1]->SetBinContent(i, highValue);
		allValues.erase(allValues.begin(), allValues.end());
    }
	
	//Delete everything except first two which are the 16percent and 82 percent and then the last one which is baseline
	histVector.erase(histVector.begin()+2, histVector.end()-1);

	for (int i = 0; i <= histVector[2]->GetNbinsX(); i++)
    {
		
		//std::cout<<histVector[0]->GetBinContent(i)<<"\t"<<histVector[1]->GetBinContent(i)<<"\t"<<histVector[2]->GetBinContent(i)<<"\n";
        std::cout<<histVector[2]->GetBinContent(i)<<",";
        
    }

    std::vector<TH1*> histVectors;
    histVectors.push_back(histVector[0]);

    TCanvas *canvas = plotFormatter->noScaleSimpleSuperImposedHist(histVector, colors, names, xTitle, yTitle); 
    canvas->SetLogy(false);

}
  