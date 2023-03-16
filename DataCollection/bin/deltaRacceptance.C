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
  TH1* alldeltaRHist=dynamic_cast<TH1*>(allGenSimdeltaR->Get("Jet Detected_hists/Jet Detected Gen Sim Delta R")); 
  TH1* deltaRfilteredHist=dynamic_cast<TH1*>(genSimdeltaRaccepted->Get("No Jets_hists/No Jets Gen Sim Delta R")); 
  
  // scale the pass trigger histogram 
  
  std::cerr << " Accpetence Ratio: " << (deltaRfilteredHist->GetEntries() / alldeltaRHist->GetEntries() ) << std::endl;

  //passTriggerHist->Scale((allEventsHist->GetEntries()) / (allEventsHist->Integral()*passTriggerHist->GetEntries())); 
  //passTriggerHist->Scale((passTriggerHist->GetEntries()*allEventsHist->Integral()) / (passTriggerHist->Integral()*allEventsHist->GetEntries())); //makes ratios match, doesn't fix hists

  //std::cerr << "(AFTER SCALING) Ratio of Integrals: " << (passTriggerHist->Integral() / allEventsHist->Integral() ) << std::endl;
  //std::cerr << "(AFTER SCALING) Ratio of Number of Entries: " << (passTriggerHist->GetEntries() / allEventsHist->GetEntries() ) << std::endl;

  //divide histograms
  //TH1 *triggerEfficiencyHist = (TH1*)passTriggerHist->Clone("triggerEfficiencyHist");
  //triggerEfficiencyHist->Divide(passTriggerHist, allEventsHist);

  
  deltaRacceptanceHist->Divide(alldeltaRHist);
  
  deltaRacceptanceHist->GetXaxis()->SetTitle("Delta R");
  deltaRacceptanceHist->GetYaxis()->SetTitle("Percentage of Events Accepted");
  deltaRacceptanceHist->SetTitle("Delta R Acceptance");
  deltaRacceptanceHist->Draw();
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

TH1* getHist(string name, TDirectory* directory);
void process(TH1* hist, std::string canvasName, std::string xaxis, std::string yaxis);

void deltaRacceptance()
{
  
    TFile* dRHists = TFile::Open("test.root");
    
    TDirectory* detected = (TDirectory*)dRHists->Get("Jet Detected_hists;1");
    TDirectory* no_hists = (TDirectory*)dRHists->Get("No Jets_hists;1");
    
    string filteredHist = "Jet Detected Gen Sim Delta R";
    string allHist = "No Jets Gen Sim Delta R";
    
    detected->cd();
    
    auto filteredPlot = getHist(filteredHist.c_str(), detected);

    dRHists->cd();
    no_hists->cd();

    auto allPlot =  getHist(allHist.c_str(), no_hists);

    dRHists->cd();

    auto acceptedHist = (TH1D*) filteredPlot->Clone();
    acceptedHist->Divide(allPlot);

    string canvasName = "Delta R Acceptance";
    string xaxis = "Delta R";
    string yaxis = "Percentage of Events Accepted";
    process(acceptedHist, canvasName, xaxis, yaxis);
}


TH1* getHist(std::string name, TDirectory* directory)
    {
    auto hist = dynamic_cast<TH1*>(directory->Get(name.c_str()));
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