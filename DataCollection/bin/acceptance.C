#include <algorithm>
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include <iostream>
#include <string>
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"

using std::string;

TH1* getHist(string name, TFile* file);
void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title, std::string rootFileName);

void acceptance(string analysis, string numJetsFilter)
{
    for (int i = 0; i < numJetsFilter; i++)
    {
        TFile* dRHists = TFile::Open("test.root");
        
        string filteredHist = "Gen Sim " + analysis + " (Reconstructed" + numJetsFilter " Jets)";
        string allHist = "Gen Sim " + analysis;
        string title = analysis + " Acceptance";
        
        auto filteredPlot = getHist(filteredHist.c_str(), dRHists);
        auto allPlot =  getHist(allHist.c_str(), dRHists);
        auto acceptedHist = (TH1D*) filteredPlot->Clone(); 
        acceptedHist->Divide(allPlot);

        string canvasName = analysis + " Acceptance (" + numJetsFilter " Jet)";
        string xaxis = analysis;
        string yaxis = "Percentage of Events Accepted";
        process(acceptedHist, canvasName, xaxis, yaxis, title, analysis + "Acceptance.root");
    } 
    
}


TH1* getHist(std::string name, TFile* file)
    {
    auto hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
    if (!hist)
        throw std::runtime_error(name + " not found!");
    return hist;
}

void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title, std::string rootFileName){
    TFile* refinedOutput = TFile::Open(rootFileName, "UPDATE");
    TCanvas* c = new TCanvas(canvasName.c_str(), canvasName.c_str());
    hist->SetStats(kFALSE);
    hist->GetXaxis()->SetTitle(xaxis.c_str());
    hist->GetYaxis()->SetTitle(yaxis.c_str());
    hist->SetTitle(title.c_str());
    hist->Write();
    hist->Draw();
    c->Update();
    c->Write();
    c->SaveAs((canvasName + ".png").c_str());
}