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
void plotTwoPlots(string hist1, string hist2, string canvasName, TFile* file1, string xaxis, string yaxis, string legend1, string legend2, double legendLeftSide = .4);

void ptPlotRefineOutput()
{
  gStyle->SetOptStat(0);
  TFile* genOutput = TFile::Open("genOutput.root");
  //TFile* outputPercentDiff = TFile::Open("refinedOutputUpDown20binsZoomed.root", "recreate");
  //TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaledZoomedQuadFit.root", "recreate");
  //TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaledZoomed.root", "recreate");
  TFile* outputPercentDiff = TFile::Open("default.root", "recreate");

  plotTwoPlots("GenSim Transverse Momentum Pasted;1", "Reco Transverse Momentum Pasted;1", "Transverse Momentum Distribution for Muon Events Updated (Fixed)", genOutput, "Transverse Momentum", "Number of Events", "GenSimMuons", "RecoMuons");

  //plotTwoPlots("RecoPt", "RecoSignFlipsPt", "Pt measurement", genOutput, "p_T", "", "Correct charge", "Incorrect charge");
  //plotTwoPlots("RecoEta", "RecoSignFlipsEta", "Eta measurement", genOutput, "#eta", "", "Correct charge", "Incorrect charge");
  //  c1->Close();

//  TCanvas * c1 = new TCanvas("Collins Soper Angle", "Collins Soper Angle");
//  TCanvas * c2 = new TCanvas("Collins Soper Angle Incorrect Order", "Collins Soper Angle Incorrect Order");
//  TCanvas * c3 = new TCanvas("Collins Soper Angle Difference", "Collins Soper Angle Difference");
//  TCanvas * c4 = new TCanvas("Random Order Collins Soper Angle", "Random Order Collins Soper Angle");
//  TCanvas * c5 = new TCanvas("High pT Collins Soper Angle", "High pT Collins Soper Angle"); 
//  TCanvas * c6 = new TCanvas("Reco Collins Soper Angle", "Reco Collins Soper Angle"); 
//  TCanvas * c7 = new TCanvas("Reco Incorrect Collins Soper Angle", "Reco Incorrect Collins Soper Angle"); 
//  TCanvas * c8 = new TCanvas("Collins Soper Angle Sign Flips Difference between Gen Reco Random", "Collins Soper Angle Sign Flips Difference between Gen Reco Random"); 
//  TCanvas * c9 = new TCanvas("Collins Soper Angle Sign Flips Difference between Gen Reco High pT", "Collins Soper Angle Sign Flips Difference between Gen Reco High pT"); 
//  TCanvas * c10 = new TCanvas("Sign Flips Reco pT", "Sign Flips Reco pT"); 
//  TCanvas * c11 = new TCanvas("Sign Flips Reco Phi", "Sign Flips Reco Phi"); 
//  TCanvas * c12 = new TCanvas("Sign Flips Reco Eta", "Sign Flips Reco Eta"); 
// TCanvas * c13 = new TCanvas("Reco pT", "Reco pT"); 
//  TCanvas * c14 = new TCanvas("Reco Phi", "Reco Phi"); 
//  TCanvas * c15 = new TCanvas("Reco Eta", "Reco Eta"); 
//
//
//      //collins soper angle histograms
//  std::vector<TCanvas*> canvi = {c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15};
//  std::vector<std::string> histNames = {"histCollinsSoper", "histIncorrectCollinsSoper", "histDifferenceAngle", "histRandomCollinsSoper", "histHighPtCollinsSoper", "RecohistCollinsSoper", "RecohistIncorrectCollinsSoper","GenRecoDifferenceSignFlipsRandom","GenRecoDifferenceSignFlipsHighPt","RecoSignFlipsPt","RecoSignFlipsPhi","RecoSignFlipsEta","RecoPt","RecoPhi","RecoEta"};
//      //loop over all the histograms
//      for(int c = 0; c < histNames.size(); c++)
//	{
//	  canvi[c]->cd();
//	  TH1D* histCollins=dynamic_cast<TH1D*>(genOutput->Get(histNames[c].c_str()));
//	  if (c==0 || c==1 || c==5 || c==6)
//	    {
//	      histCollins->GetXaxis()->SetTitle("Collins Soper Angle");
//	    }
//	  else if (c==9||c==12)
//	    {
//	      histCollins->GetXaxis()->SetTitle("pT");
//	    }
//	  else if (c==10||c==13)
//	    {
//	      histCollins->GetXaxis()->SetTitle("Phi");
//	    }
//	  else if (c==11||c==14)
//	    {
//	      histCollins->GetXaxis()->SetTitle("Eta");
//	    }
//	  else{
//	    
//	    histCollins->GetXaxis()->SetTitle("cos #theta Difference");
//	  }
//
//	  histCollins->GetYaxis()->SetTitle("Number of Events");
//	  //std::cout<<c<<std::endl;
//
//	  //set titles for each histogram
//	  if (c==0)
//	    {
//	      histCollins->SetTitle("Collins Soper Angle Correctly Ordered");
//	    }
//	  else if (c==1)
//	    {
//	      histCollins->SetTitle("Collins Soper Angle Incorrectly Ordered");
//	    }
//	  else if (c==2)
//	    {
//	      histCollins->SetTitle("Collins Soper Angle Difference Between Ordering");
//	    }
//	  else if (c==3)
//	    {
//	      histCollins->SetTitle("Collins Soper Angle Randomly Ordered");
//	    }
//	  else if (c==4)
//	    {
//	      histCollins->SetTitle("Collins Soper Angle High Pt is First Particle");
//	    }
//	  else if (c==5)
//	    {
//	      histCollins->SetTitle("Reco Collins Soper Correctly Ordered");
//	    }
//	  else if (c==6)
//	    {
//	      histCollins->SetTitle("Reco Collins Soper Incorrectly Ordered");
//	    }
//	  else if (c==7)
//	    {
//	      histCollins->SetTitle("Collins Soper Angle Sign Flips Difference between Gen and Reco Randomly Ordered");
//	    }
//	  else if (c==8)
//	    {
//	      histCollins->SetTitle("Collins Soper Angle Sign Flips Difference between Gen and Reco High Pt is First Particle");
//	    }
//	  else if (c==9)
//	    {
//	      histCollins->SetTitle("Sign Flips Reco Pt");
//	    }
//	  else if (c==10)
//	    {
//	      histCollins->SetTitle("Sign Flips Reco Phi");
//	    }
//	  else if (c==11)
//	    {
//	      histCollins->SetTitle("Sign Flips Reco Eta");
//	    }
//	  else if (c==12)
//	    {
//	      histCollins->SetTitle("Reco Pt");
//	    }
//	  else if (c==13)
//	    {
//	      histCollins->SetTitle("Reco Phi");
//	    }
//	  else if (c==14)
//	    {
//	      histCollins->SetTitle("Reco Eta");
//	    }
//	  else
//	    
//	    {
//	      std::cout<<"No title set"<<std::endl;
//	    }
//	    
//	  histCollins->SetFillColor(30);
//	  histCollins->Draw("HIST");
//	  canvi[c]->Update();
//	  canvi[c]->Write(); 
//	  canvi[c]->Close();
//	  
//    }
}


TH1* getHist(std::string name, TFile* file)
{
  auto hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
  if (!hist)
    throw std::runtime_error(name + " not found!");
  return hist;
}

void plotTwoPlots(string hist1, string hist2, string canvasName, TFile* file1, string xaxis, string yaxis, string legend1, string legend2, double legendLeftSide)
{
  TCanvas* c1 = new TCanvas(canvasName.c_str(), canvasName.c_str());

  auto plot1 = getHist(hist1.c_str(), file1);
  auto plot2 = getHist(hist2.c_str(), file1);
  // auto plot3 = getHist(hist3.c_str(), file3);
  // auto plot4 = getHist(hist4.c_str(), file4);
  // auto plot5 = getHist(hist5.c_str(), file5);

  c1->SetLogy(true);
  const char* canvasTitle = "Transverse Momentum Distribution for Muon Events Updated";
  plot1->SetTitle(canvasTitle);

  plot2->Scale(plot1->Integral() / plot2->Integral());
  // plot3->Scale(plot1->Integral() / plot3->Integral());
  // plot4->Scale(plot1->Integral() / plot4->Integral());
  // plot5->Scale(plot1->Integral() / plot5->Integral());

  plot1->SetLineColor(kBlue);
  plot2->SetLineColor(kRed);
  // plot3->SetLineColor(kBlack);
  // plot4->SetLineColor(kOrange);
  // plot5->SetLineColor(kGreen);

  TAxis* xaxis1 = plot1->GetXaxis();
  TAxis* xaxis2 = plot2->GetXaxis();
  // TAxis* xaxis3 = plot3->GetXaxis();
  // TAxis* xaxis4 = plot4->GetXaxis();
  // TAxis* xaxis5 = plot5->GetXaxis();

  xaxis1->SetLimits(0, 3000);
  xaxis2->SetLimits(0, 3000);
  // xaxis3->SetLimits(0, 3000);
  // xaxis4->SetLimits(0, 3000);
  // xaxis5->SetLimits(0, 3000);

  plot1->GetXaxis()->SetTitle(xaxis.c_str());
  plot1->GetYaxis()->SetTitle(yaxis.c_str());

  plot1->Draw("HIST");
  plot2->Draw("SAMEHIST");
  // plot3->Draw("SAMEHIST");
  // plot4->Draw("SAMEHIST");
  // plot5->Draw("SAMEHIST");

  double max = std::max(plot1->GetMaximum(), plot2->GetMaximum());
  // double plot3max = std::max(max, plot3->GetMaximum());
  // double plot4max = std::max(max, plot4->GetMaximum());
  // double plot5max = std::max(max, plot5->GetMaximum());
  plot1->SetMaximum(max * 1.1);

  auto legend = new TLegend(legendLeftSide, .8, legendLeftSide + .3, .9);
  legend->AddEntry(plot1, legend1.c_str(), "L");
  legend->AddEntry(plot2, legend2.c_str(), "L");
  // legend->AddEntry(plot3, legend3.c_str(), "L");
  // legend->AddEntry(plot4, legend4.c_str(), "L");
  // legend->AddEntry(plot5, legend5.c_str(), "L");
  legend->Draw();

  c1->Update();
  c1->Write();
  c1->SaveAs((canvasName + ".png").c_str());
}
