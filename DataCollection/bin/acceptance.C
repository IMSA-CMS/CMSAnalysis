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
//void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title, std::string rootFileName);
void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title);    

//analysis: e.g. Gamma, Delta R
//numJetsFilter: number of lepton jets to filter for
//inputFile: input root file name
//higgsMass: higgs mass of file running on
void acceptance(std::string analysis, std::string numJetsFilter, std::string inputFile, std::string higgsMass)
{
    std::string rootFileName = analysis + "Acceptance" + higgsMass + ".root";
    TFile* refinedOutput = TFile::Open(rootFileName.c_str(), "RECREATE");
    for (int i = 1; i <= std::stoi(numJetsFilter); i++)
    {
        TFile* dRHists = TFile::Open(inputFile.c_str());
        
        string filteredHist = "Gen Sim " + analysis + " (Reconstructed " + std::to_string(i) + " Jets)";
        string allHist = "Gen Sim " + analysis;
        string title = analysis + " Acceptance (" + std::to_string(i) + " Jet, Higgs Mass " + higgsMass + ")";
        
        auto filteredPlot = getHist(filteredHist.c_str(), dRHists);
        auto allPlot =  getHist(allHist.c_str(), dRHists);
        auto acceptedHist = (TH1D*) filteredPlot->Clone(); 
        acceptedHist->Divide(allPlot);

        string canvasName = analysis + " Acceptance (" + std::to_string(i) + " Jet, Higgs Mass " + higgsMass + ")";
        string xaxis = analysis;
        string yaxis = "Percentage of Events Accepted";
        //std::string rootFileName = analysis + "Acceptance.root";
        //process(acceptedHist, canvasName, xaxis, yaxis, title, rootFileName);
        refinedOutput->cd();
        process(acceptedHist, canvasName, xaxis, yaxis, title);
    } 
    
}

TH1* getHist(std::string name, TFile* file)
    {
    auto hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
    if (!hist)
        throw std::runtime_error(name + " not found!");
    return hist;
}

//void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title, std::string rootFileName){
void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis, std::string title){    
    //TFile* refinedOutput = TFile::Open(rootFileName.c_str(), "UPDATE");
    TCanvas* c = new TCanvas(canvasName.c_str(), canvasName.c_str());
    hist->SetStats(kFALSE);
    hist->GetXaxis()->SetTitle(xaxis.c_str());
    hist->GetYaxis()->SetTitle(yaxis.c_str());
    hist->SetTitle(title.c_str());
    hist->Write();
    hist->Draw();
    c->Update();
    c->Write();
    c->SaveAs((canvasName + " " +  ".png").c_str());
}