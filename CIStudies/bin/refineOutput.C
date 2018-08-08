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
  TCanvas * c1 = new TCanvas("c1", "Gen Relative Difference");
  TCanvas * c2 = new TCanvas("c2", "Reco Relative Difference");
  std::string histName;
  std::string word = "histrel";
  TLatex T; 
  TLatex T1; 
  if (genOutput)
    {
      
      std::string scales [2] = {"up", "dn"};
      std::string levels [2] = {"G", "R"}; 
      double kappa = 0.00005; 
      for(int a = 0; a < 2; a++)
	{
	  
	  if (a == 0)
	    {
	      c1->cd();
	    }

	  else
	    {
	      c2->cd();
	    }
	  auto scaleLegend = new TLegend(0.16,0.63,0.35,0.85);
	  for(int b = 0; b < 2; b++)
	    {
	      
	      histName = levels[a] + word + scales[b];
	      //histName = levels[a] + word + scales[0] + scales[1];
	      //histName = levels[a] + word + scales[0] + scales[1];
	      TH1D* percentdiff=dynamic_cast<TH1D*>(genOutput->Get(histName.c_str()));
	  
	  
	      if(percentdiff)
		{
		  
		  percentdiff->SetStats(kFALSE); //take out statistics box
		  
		  percentdiff->SetLineColor(b+1);
		  percentdiff->GetXaxis()->SetTitle("Invariant Mass (GeV)");
		  percentdiff->GetYaxis()->SetTitle("Relative Difference");
		  percentdiff->GetXaxis()->SetRangeUser(1000, 2400); //only for zoomed in graphs
		  percentdiff->GetYaxis()->SetTitleOffset(1.4);
		  double bincontent = percentdiff->GetBinContent(51); //get bin content at 1000 GeV 
		  //for(int c = 51; 
		  // double binerror = percentdiff->GetBinError()
		  double scalefactor = .015/bincontent;
		  percentdiff->Scale(scalefactor); //scale 1.5% at 1000 GeV
		  T.SetTextSize(0.03); 
		  T.DrawLatex(1100, 0.035, "#kappa = 0.00005");
		  T.DrawLatex(1100, 0.04, "#lambda = 16 TeV");
		  percentdiff->SetOption("E");
		  
		      
		  if (b==0)
		    {
		      percentdiff->SetLineColor(kBlue);
		      
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
		      T.DrawLatex(1100, 0.03, equation.c_str());
		      //------------------------------------------------
		      scaleLegend->AddEntry(percentdiff, "Scaled-up", "l");
		    }
		  else
		    {
		      percentdiff->SetLineColor(kRed);
		      percentdiff->Draw("SAME");
		      scaleLegend->AddEntry(percentdiff, "Scaled-down", "l");
		    }

		  
		  if(a==0)
		    {
		      percentdiff->SetTitle("Gen Relative Difference, 1.5% at 1000 GeV"); //when scaling to 1.5%
		      //percentdiff->SetTitle("Gen Relative Difference Mu Scaled Up AntiMu Scaled Down "); 
		      //percentdiff->SetTitle("Gen Relative Difference"); 
		    }
		  else
		    {
		      percentdiff->SetTitle("Reco Relative Difference, 1.5% at 1000 GeV"); //when scaling to 1.5%
		      //percentdiff->SetTitle("Reco Relative Difference Mu Scaled Up AntiMu Scaled Down");
		      //percentdiff->SetTitle("Reco Relative Difference"); 
		    }
		  
			
		}
	      
	    }
          

	  scaleLegend->Draw();
	  if (a == 0)
	    {
	      
	      c1->Update();

	    }
	  else
	    {
	      
	      c2->Update();
	    }
	}
      
      c1->Write();
      c2->Write();
    }
}
