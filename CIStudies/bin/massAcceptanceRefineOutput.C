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
void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis);

void massAcceptanceRefineOutput()
{
  
    TFile* MVAHists = TFile::Open("output.root");
    
    string hist1 = "Mass vs Total Events";
    string hist2 = "Mass vs Accepted Events";
    string hist3 = "Mass vs Acceptance";
    
    
    auto plot1 = getHist(hist1.c_str(), MVAHists);
    auto plot2 =  getHist(hist2.c_str(), MVAHists);
    auto finalHist = getHist(hist3.c_str(), MVAHists);
    auto tempBool = finalHist->Divide(plot1);

    string canvasName1 = "Mass VS Total Events";
    string canvasName2 = "Mass VS Accepted Events";
    string canvasName3 = "Mass VS Acceptance";
    string xaxis = "Mass(GeV)";
    string yaxisOriginal = "Events";
    string yaxisFinal = "Acceptance";
    process(plot1, canvasName1, xaxis, yaxisOriginal);
    process(plot2, canvasName2, xaxis, yaxisOriginal);
    process(finalHist, canvasName3, xaxis, yaxisFinal);
}


TH1* getHist(std::string name, TFile* file)
    {
    auto hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
    if (!hist)
        throw std::runtime_error(name + " not found!");
    return hist;
}

void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis){
    TFile* refinedOutput = TFile::Open("massAcceptanceRefinedOutput.root", "UPDATE");
    TCanvas* c = new TCanvas(canvasName.c_str(), canvasName.c_str());
    hist->SetStats(kFALSE);
    hist->GetXaxis()->SetTitle(xaxis.c_str());
    hist->GetYaxis()->SetTitle(yaxis.c_str());
    hist->Write();
    hist->Draw();
    c->Update();
    c->Write();
    c->SaveAs((canvasName + ".png").c_str());
}