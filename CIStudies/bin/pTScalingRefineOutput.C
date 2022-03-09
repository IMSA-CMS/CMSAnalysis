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

void addLabels(int a, int b, TH1D* percentdiff, int xPos, double yPos, std::string title)
{
  TLatex T; 
  TLatex T1;
  
  if(percentdiff)
    {
		  
      percentdiff->SetStats(kFALSE); //take out statistics box
		  
      percentdiff->SetLineColor(b+1);
      percentdiff->GetXaxis()->SetTitle("Invariant Mass (GeV)");
      percentdiff->GetYaxis()->SetTitle("Relative Difference");
      
      percentdiff->GetYaxis()->SetTitleOffset(1.4);
      
      T.SetTextSize(0.03); 
      T.DrawLatex(xPos, yPos, "#kappa = 0.00005");
      T.DrawLatex(xPos, yPos+(yPos/5), "#lambda = 16 TeV");
      percentdiff->SetOption("E");
		  
		  
      if (b==0)
	{
	  percentdiff->SetLineColor(kRed);
		      
	  percentdiff->Draw("");
		      
	  //-----These lines only for quadratic fit-----------
	  auto func = new TF1("myfunction", "pol2", 0, 3000);
	  percentdiff->Fit(func);
	  func->SetLineColor(kBlack);
	  func->Draw("SAME");
	  double p0 = func->GetParameter(0);
	  double p1 = func->GetParameter(1);
	  //double p2 = func->GetParameter(2); 
	  std::string equation = std::to_string(p0)+"x^{2} + "+std::to_string(p1)+"x "; 
	  T.DrawLatex(xPos, yPos-(yPos/5), equation.c_str());
	  //------------------------------------------------
	  // scaleLegend->AddEntry(percentdiff, "Scaled-up", "l");
	}
      else
	{
	  percentdiff->SetLineColor(kBlue);
	  percentdiff->Draw("SAME");
	  // scaleLegend->AddEntry(percentdiff, "Scaled-down", "l");
	}

		  
      if(a==0)
	{
	  percentdiff->SetTitle((std::string("Gen")+title).c_str()); //when scaling to 1.5%
	  percentdiff->Write();
	  //percentdiff->SetTitle("Gen Relative Difference Mu Scaled Up AntiMu Scaled Down "); 
	  //percentdiff->SetTitle("Gen Relative Difference"); 
	}
      else
	{ 
	  percentdiff->SetTitle((std::string("Reco")+title).c_str()); //when scaling to 1.5%
	   percentdiff->Write(); 
	  //percentdiff->SetTitle("Reco Relative Difference Mu Scaled Up AntiMu Scaled Down");
	  //percentdiff->SetTitle("Reco Relative Difference"); 
	}
		  
			
    }
	
}



void refineOutput()
{
  
  TFile* genOutput = TFile::Open("genOutput.root");
  //TFile* outputPercentDiff = TFile::Open("refinedOutputUpDown20binsZoomed.root", "recreate");
  //TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaledZoomedQuadFit.root", "recreate");
  //TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaledZoomed.root", "recreate");
  TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaled.root", "recreate");
  TCanvas * c = new TCanvas("c", "Relative Difference"); 
  c->Divide(2,2); 
  TCanvas * c5 = new TCanvas("c5", "Relative Difference Up Down"); 
  c5->Divide(2,2); 
  TCanvas * c3 = new TCanvas("c3", "Scaled Gen Relative Difference");
  TCanvas * c4 = new TCanvas("c4", "Scaled Reco Relative Difference");
  std::string histName;
  std::string histName1; 
  std::string word = "histrel"; 
  if (genOutput)
    {
      
      std::string scales [2] = {"up", "dn"};
      std::string levels [2] = {"G", "R"}; 
      double kappa = 0.00005; 
      
      for(int a = 0; a < 2; a++)
	{
	  for(int b = 0; b < 2; b++)
	    {
	      
	      histName = levels[a] + word + scales[b];
	      histName1 = levels[a] + word + scales[0] + scales[1];
	      TH1D* percentdiff=dynamic_cast<TH1D*>(genOutput->Get(histName.c_str()));
	      TH1D* percentdiffupdown=dynamic_cast<TH1D*>(genOutput->Get(histName1.c_str()));
	      TH1D* percentdiffupdownzoom =(TH1D*) percentdiffupdown ->Clone();
	      TH1D* percentdiffScaled =(TH1D*) percentdiff ->Clone();
	      TH1D* percentdiffZoom =(TH1D*) percentdiff ->Clone();
	      if (percentdiff)
		{
		  double bincontent = percentdiff->GetBinContent(51); //get bin content at 1000 GeV 
		  double scalefactor = .015/bincontent;
		  percentdiffScaled->Scale(scalefactor); //scale 1.5% at 1000 GeV
		  percentdiffScaled->GetXaxis()->SetRangeUser(1000, 2400);
		  percentdiffupdownzoom->GetXaxis()->SetRangeUser(1000, 2400);
		  percentdiffZoom->GetXaxis()->SetRangeUser(1000, 2400); //only for zoomed in graphs
		  if (a == 0)
		    {
		      c->cd(1);
		      gPad->SetTickx(2); 
		    }

		  else
		    {
		      c->cd(2);
		      gPad->SetTicky(2); 
		    }
		  addLabels(a, b, percentdiff, 500, 4, "Relative Difference");
		  if (a == 0)
		    {
		      c->cd(3);
		      gPad->SetTickx(2); 
		    }

		  else
		    {
		      c->cd(4);
		      gPad->SetTicky(2); 
		    }
		  addLabels(a, b, percentdiffZoom, 1100, 0.2, "Relative Difference");
		  
		  if (a == 0)
		    {
		      c3->cd();
		    }

		  else
		    {
		      c4->cd();
		    }
		  addLabels(a, b, percentdiffScaled, 1100, 0.04, "Relative Difference, 1.5% at 1000 GeV ");
		   if (a == 0)
		    {
		      c5->cd(1);
		      gPad->SetTickx(2); 
		    }

		  else
		    {
		      c5->cd(2);
		      gPad->SetTicky(2); 
		    }
		  addLabels(a, b, percentdiffupdown, 500, 0.15, "Relative Difference Muon Scaled Up Antimuon Scaled Down");
		  if (a == 0)
		    {
		      c5->cd(3);
		      gPad->SetTickx(2); 
		    }

		  else
		    {
		      c5->cd(4);
		      gPad->SetTicky(2); 
		    }
		  addLabels(a, b, percentdiffupdownzoom, 1100, 0.01, "Relative Difference Muon Scaled Up Antimuon Scaled Down");
		 
		}
	       

	    }  
      
	  if (a == 0)
	    {
	      c3->Update(); 
	 
	      //percentdiff->Write(); 

	    }
	  else
	    { 
	      c4->Update(); 
	      //percentdiff->Write(); 
	    }
	  
	  
	}
      c3->Write(); 
      c4->Write(); 
      c->Write(); 
      c5->Write(); 
      //outputPercentDiff->Close();
      
    }
}
