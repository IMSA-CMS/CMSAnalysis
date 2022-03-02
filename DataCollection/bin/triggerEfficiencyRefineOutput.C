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

void triggerEfficiencyRefineOutput()
{
  TFile* leptonJetNoTriggersOutput = TFile::Open("noTriggersLeptonJetHists_5_files.root");
  TFile* leptonJetAllTriggersOutput = TFile::Open("allTriggersLeptonJetHists_5_files.root");
  
  TCanvas* histogramCanvas = new TCanvas("c", "Trigger Efficiency");
  //histogramCanvas->cd();


  //get the histograms from the root file
  TH1* passTriggerHist=dynamic_cast<TH1*>(leptonJetAllTriggersOutput->Get("Reconstructed First Muon Transverse MomentumM300")); 
  TH1* allEventsHist=dynamic_cast<TH1*>(leptonJetNoTriggersOutput->Get("Reconstructed First Muon Transverse MomentumM300")); 
  
  // scale the pass trigger histogram 
  
  std::cerr << "Ratio of Integrals: " << (passTriggerHist->Integral() / allEventsHist->Integral() ) << std::endl;
  std::cerr << "Ratio of Number of Entries: " << (passTriggerHist->GetEntries() / allEventsHist->GetEntries() ) << std::endl;

  //passTriggerHist->Scale((allEventsHist->GetEntries()) / (allEventsHist->Integral()*passTriggerHist->GetEntries())); 
  //passTriggerHist->Scale((passTriggerHist->GetEntries()*allEventsHist->Integral()) / (passTriggerHist->Integral()*allEventsHist->GetEntries())); //makes ratios match, doesn't fix hists

  std::cerr << "(AFTER SCALING) Ratio of Integrals: " << (passTriggerHist->Integral() / allEventsHist->Integral() ) << std::endl;
  std::cerr << "(AFTER SCALING) Ratio of Number of Entries: " << (passTriggerHist->GetEntries() / allEventsHist->GetEntries() ) << std::endl;

  //divide histograms
  //TH1 *triggerEfficiencyHist = (TH1*)passTriggerHist->Clone("triggerEfficiencyHist");
  //triggerEfficiencyHist->Divide(passTriggerHist, allEventsHist);
  TH1 *triggerEfficiencyHist = (TH1*)passTriggerHist->Clone();
  triggerEfficiencyHist->Divide(allEventsHist);
  
  triggerEfficiencyHist->GetXaxis()->SetTitle("Pt");
  triggerEfficiencyHist->GetYaxis()->SetTitle("Trigger Efficiency");
  triggerEfficiencyHist->SetTitle("Reconstructed 1st Muon Trigger Efficiency with All Three Triggers");
  triggerEfficiencyHist->Draw();
}














