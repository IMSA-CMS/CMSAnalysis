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

void refineOutput()
{
  
  TFile* genOutput = TFile::Open("genOutput.root");
  //TFile* outputPercentDiff = TFile::Open("refinedOutputUpDown20binsZoomed.root", "recreate");
  //TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaledZoomedQuadFit.root", "recreate");
  //TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaledZoomed.root", "recreate");
  TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaled.root", "recreate");
  TCanvas * c1 = new TCanvas("Collins Soper Angle", "Collins Soper Angle");
  TCanvas * c2 = new TCanvas("Collins Soper Angle Incorrect Order", "Collins Soper Angle Incorrect Order");
  TCanvas * c3 = new TCanvas("Collins Soper Angle Difference", "Collins Soper Angle Difference");
  TCanvas * c4 = new TCanvas("Random Order Collins Soper Angle", "Random Order Collins Soper Angle");
  TCanvas * c5 = new TCanvas("High pT Collins Soper Angle", "High pT Collins Soper Angle"); 
  TCanvas * c6 = new TCanvas("Reco Collins Soper Angle", "Reco Collins Soper Angle"); 
  TCanvas * c7 = new TCanvas("Reco Incorrect Collins Soper Angle", "Reco Incorrect Collins Soper Angle"); 
  TCanvas * c8 = new TCanvas("Collins Soper Angle Sign Flips Difference between Gen Reco Random", "Collins Soper Angle Sign Flips Difference between Gen Reco Random"); 
  TCanvas * c9 = new TCanvas("Collins Soper Angle Sign Flips Difference between Gen Reco High pT", "Collins Soper Angle Sign Flips Difference between Gen Reco High pT"); 
  TCanvas * c10 = new TCanvas("Sign Flips Reco pT", "Sign Flips Reco pT"); 
  TCanvas * c11 = new TCanvas("Sign Flips Reco Phi", "Sign Flips Reco Phi"); 
  TCanvas * c12 = new TCanvas("Sign Flips Reco Eta", "Sign Flips Reco Eta"); 
  TCanvas * c13 = new TCanvas("Reco pT", "Reco pT"); 
  TCanvas * c14 = new TCanvas("Reco Phi", "Reco Phi"); 
  TCanvas * c15 = new TCanvas("Reco Eta", "Reco Eta"); 
  TCanvas * c16 = new TCanvas("Reco Correct Sign Pt", "Reco Correct Sign Pt");
  TCanvas * c17 = new TCanvas("Reco Correct Sign Eta", "Reco Correct Sign Eta");
  TCanvas * c18 = new TCanvas("Reco Wrong Sign Pt", "Reco Wrong Sign Pt");
  TCanvas * c19 = new TCanvas("Reco Wrong Sign Eta", "Reco Wrong Sign Eta");


      //collins soper angle histograms
  std::vector<TCanvas*> canvi = {c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19};
  std::vector<std::string> histNames = {"histCollinsSoper", "histIncorrectCollinsSoper", "histDifferenceAngle", "histRandomCollinsSoper", "histHighPtCollinsSoper", "RecohistCollinsSoper", "RecohistIncorrectCollinsSoper","GenRecoDifferenceSignFlipsRandom","GenRecoDifferenceSignFlipsHighPt","RecoSignFlipsPt","RecoSignFlipsPhi","RecoSignFlipsEta","RecoPt","RecoPhi","RecoEta","RecoCorrectSignPt","RecoCorrectSignEta","RecoWrongSignPt","RecoWrongSignEta"};
      //loop over all the histograms
      for(int c = 0; c < histNames.size(); c++)
	{
	  canvi[c]->cd();
	  TH1D* histCollins=dynamic_cast<TH1D*>(genOutput->Get(histNames[c].c_str()));
	  if (c==0 || c==1 || c==5 || c==6)
	    {
	      histCollins->GetXaxis()->SetTitle("Collins Soper Angle");
	    }
	  else if (c==9||c==12)
	    {
	      histCollins->GetXaxis()->SetTitle("pT");
	    }
	  else if (c==10||c==13)
	    {
	      histCollins->GetXaxis()->SetTitle("Phi");
	    }
	  else if (c==11||c==14)
	    {
	      histCollins->GetXaxis()->SetTitle("Eta");
	    }
	  else{
	    
	    histCollins->GetXaxis()->SetTitle("Collins Soper Angle Difference");
	  }

	  histCollins->GetYaxis()->SetTitle("Number of Events");
	  //std::cout<<c<<std::endl;

	  //set titles for each histogram
	  if (c==0)
	    {
	      histCollins->SetTitle("Collins Soper Angle Correctly Ordered");
	    }
	  else if (c==1)
	    {
	      histCollins->SetTitle("Collins Soper Angle Incorrectly Ordered");
	    }
	  else if (c==2)
	    {
	      histCollins->SetTitle("Collins Soper Angle Difference Between Ordering");
	    }
	  else if (c==3)
	    {
	      histCollins->SetTitle("Collins Soper Angle Randomly Ordered");
	    }
	  else if (c==4)
	    {
	      histCollins->SetTitle("Collins Soper Angle High Pt is First Particle");
	    }
	  else if (c==5)
	    {
	      histCollins->SetTitle("Reco Collins Soper Correctly Ordered");
	    }
	  else if (c==6)
	    {
	      histCollins->SetTitle("Reco Collins Soper Incorrectly Ordered");
	    }
	  else if (c==7)
	    {
	      histCollins->SetTitle("Collins Soper Angle Sign Flips Difference between Gen and Reco Randomly Ordered");
	    }
	  else if (c==8)
	    {
	      histCollins->SetTitle("Collins Soper Angle Sign Flips Difference between Gen and Reco High Pt is First Particle");
	    }
	  else if (c==9)
	    {
	      histCollins->SetTitle("Sign Flips Reco Pt");
	    }
	  else if (c==10)
	    {
	      histCollins->SetTitle("Sign Flips Reco Phi");
	    }
	  else if (c==11)
	    {
	      histCollins->SetTitle("Sign Flips Reco Eta");
	    }
	  else if (c==12)
	    {
	      histCollins->SetTitle("Reco Pt");
	    }
	  else if (c==13)
	    {
	      histCollins->SetTitle("Reco Phi");
	    }
	  else if (c==14)
	    {
	      histCollins->SetTitle("Reco Eta");
	    }
	  else if (c==15)
	    {
	     histCollins->SetTitle("Reco Correct Sign Pt");
	    }
	  else if (c==16)
	    {
	     histCollins->SetTitle("Reco Correct Sign Eta");
	    }
	  else if (c==17)
	    {
	     histCollins->SetTitle("Reco Wrong Sign Pt");
	    }
	  else if (c==18)
	    {
	     histCollins->SetTitle("Reco Wrong Sign Eta");
	    }
	  else
	    
	    {
	      std::cout<<"No title set"<<std::endl;
	    }
	    
	  histCollins->SetFillColor(30);
	  histCollins->Draw("HIST");
	  canvi[c]->Update();
	  canvi[c]->Write(); 
	  canvi[c]->Close();
	  
    }
}


