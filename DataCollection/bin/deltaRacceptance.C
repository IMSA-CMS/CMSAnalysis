/*#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include <iostream>
#include <string>
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TRandom.h"

void deltaRacceptance()
{
  TFile* genSimdeltaRaccepted = TFile::Open("test.root");
  TFile* allGenSimdeltaR = TFile::Open("test.root");
  
  TCanvas* histogramCanvas = new TCanvas("c", "Delta R Acceptance");
  //histogramCanvas->cd();


  //get the histograms from the root file
  TH1* alldeltaRHist=dynamic_cast<TH1*>(allGenSimdeltaR->Get("Gen Sim Delta R")); 
  TH1* deltaRfilteredHist=dynamic_cast<TH1*>(genSimdeltaRaccepted->Get("Gen Sim Delta R (W/ Reconstructed Jets)")); 
  
  // scale the pass trigger histogram 
  
  //std::cerr << " Accpetence Ratio: " << (deltaRfilteredHist->GetEntries() / alldeltaRHist->GetEntries() ) << std::endl;

  //passTriggerHist->Scale((allEventsHist->GetEntries()) / (allEventsHist->Integral()*passTriggerHist->GetEntries())); 
  //passTriggerHist->Scale((passTriggerHist->GetEntries()*allEventsHist->Integral()) / (passTriggerHist->Integral()*allEventsHist->GetEntries())); //makes ratios match, doesn't fix hists

  //std::cerr << "(AFTER SCALING) Ratio of Integrals: " << (passTriggerHist->Integral() / allEventsHist->Integral() ) << std::endl;
  //std::cerr << "(AFTER SCALING) Ratio of Number of Entries: " << (passTriggerHist->GetEntries() / allEventsHist->GetEntries() ) << std::endl;

  //divide histograms
  //TH1 *triggerEfficiencyHist = (TH1*)passTriggerHist->Clone("triggerEfficiencyHist");
  //triggerEfficiencyHist->Divide(passTriggerHist, allEventsHist);

  
  deltaRfilteredHist->Divide(alldeltaRHist);
  
  deltaRfilteredHist->GetXaxis()->SetTitle("Delta R");
  deltaRfilteredHist->GetYaxis()->SetTitle("Percentage of Events Accepted");
  deltaRfilteredHist->SetTitle("Delta R Acceptance");
  deltaRfilteredHist->Draw();
}*/









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

void deltaRacceptance()
{
    TFile* dRHists = TFile::Open("test.root");
    
    string filteredHist = "Gen Sim Delta R (W/ Reconstructed Jets)";
    string allHist = "Gen Sim Delta R";
    
    auto filteredPlot = getHist(filteredHist.c_str(), dRHists);
    auto allPlot =  getHist(allHist.c_str(), dRHists);
    auto acceptedHist = (TH1D*) filteredPlot->Clone(); 
    acceptedHist->Divide(allPlot);

    string canvasName = "Delta R Acceptance";
    string xaxis = "Delta R";
    string yaxis = "Percentage of Events Accepted";
    process(acceptedHist, canvasName, xaxis, yaxis);
}


TH1* getHist(std::string name, TFile* file)
    {
    auto hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
    if (!hist)
        throw std::runtime_error(name + " not found!");
    return hist;
}

void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis){
    TFile* refinedOutput = TFile::Open("deltaRacceptance.root", "UPDATE");
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