#include "TROOT.h"
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
  TH1* alldeltaRHist=dynamic_cast<TH1*>(allGenSimdeltaR->Get("___")); 
  TH1* deltaRfilteredHist=dynamic_cast<TH1*>(genSimdeltaRaccepted->Get("___")); 
  
  // scale the pass trigger histogram 
  
  std::cerr << " Accpetence Ratio: " << (deltaRfilteredHist->GetEntries() / alldeltaRHist->GetEntries() ) << std::endl;

  //passTriggerHist->Scale((allEventsHist->GetEntries()) / (allEventsHist->Integral()*passTriggerHist->GetEntries())); 
  //passTriggerHist->Scale((passTriggerHist->GetEntries()*allEventsHist->Integral()) / (passTriggerHist->Integral()*allEventsHist->GetEntries())); //makes ratios match, doesn't fix hists

  //std::cerr << "(AFTER SCALING) Ratio of Integrals: " << (passTriggerHist->Integral() / allEventsHist->Integral() ) << std::endl;
  //std::cerr << "(AFTER SCALING) Ratio of Number of Entries: " << (passTriggerHist->GetEntries() / allEventsHist->GetEntries() ) << std::endl;

  //divide histograms
  //TH1 *triggerEfficiencyHist = (TH1*)passTriggerHist->Clone("triggerEfficiencyHist");
  //triggerEfficiencyHist->Divide(passTriggerHist, allEventsHist);
  TH1 *deltaRacceptanceHist = (TH1*)deltaRfilteredHist->Clone();
  deltaRacceptanceHist->Divide(alldeltaRHist);
  
  deltaRacceptanceHist->GetXaxis()->SetTitle("Delta R");
  deltaRacceptanceHist->GetYaxis()->SetTitle("Percentage of Events Accepted");
  deltaRacceptanceHist->SetTitle("Delta R Acceptance");
  deltaRacceptanceHist->Draw();
}