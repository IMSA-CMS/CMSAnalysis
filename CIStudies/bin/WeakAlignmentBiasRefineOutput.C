#include <algorithm>
#include <cmath>
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
#include "TGraphErrors.h"

using std::string;
using std::max;

TH1* getHist(string name, TFile* file);
double calculateRelativeDifference(double scaled, double original) {return (fabs(scaled-original)/original);}
double calculateError(double scaled, double original, double sigmaScaled, double sigmaOriginal);
double calculateMidpoint(int down, int up) {return ((1.0*(down+up))/2);}
double findMaximum(double array[], int arrayLength);

void createTGraphs(string defaultHist, string scaledUpHist, string scaledDownHist, string scaledUpDownHist, string canvasName, string graphTitle, string xaxis, string yaxis, string legendUp, string legendDown, TFile* histFile, bool needScaledUpDown);
void plotTwoPlots(string hist1, string hist2, string canvasName, TFile* file1, TFile* file2, string xaxis, string yaxis, string legend1, string legend2, double legendLeftSide = .4);

void WeakAlignmentBiasRefineOutput()
{
  gStyle->SetOptStat(0);
  TFile* weakAlignBias = TFile::Open("weakAlignBiasOutput.root");
  //TFile* outputPercentDiff = TFile::Open("refinedOutputUpDown20binsZoomed.root", "recreate");
  //TFile* outputPercentDiff = TFile::Open("refinedOutput20binsNotScaledZoomedQuadFit.root", "recreate");
  //TFile* outputPercedjdsjkjdsntDiff = TFile::Open("refinedOutput20binsNotScaledZoomed.root", "recreate");
  TFile* outputPercentDiff = TFile::Open("output.root", "recreate");

  // plotTwoPlots("Reco Invariant Mass Pasted;1", "Reco Invariant Mass Pasted;1","Reco Invariant Mass Distribution for LR vs. RL", LR, RL, "Invariant Mass", "Number of Events","LRRecoMuons", "RLRecoMuons");

  bool needUpDown = false;
  createTGraphs("GenSim Invariant Mass Pasted;1", "GenSim Weak Alignment Bias Scaled Up Hist;1", "GenSim Weak Alignment Bias Scaled Down Hist;1", "GenSim Weak Alignment Bias Mu Scaled Up AntiMu Scaled Down Hist;1", "GenSim Relative Difference", "Gen Relative Difference", "Invariant Mass (GeV)", "Relative Difference", "Scaled Up", "Scaled Down", weakAlignBias, needUpDown);
  // plotTwoPlots("RecoPt", "RecoSignFlipsPt", "Pt measurement", genOutput, "p_T", "", "Correct charge", "Incorrect charge", 
	     //  .5);
  // plotTwoPlots("RecoEta", "RecoSignFlipsEta", "Eta measurement", genOutput, "#eta", "", "Correct charge", "Incorrect charge");

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

double calculateError(double scaled, double original, double sigmaScaled, double sigmaOriginal)
{
  double scaledEvents = 1.0 * scaled;
  double originalEvents = 1.0 * original;

  double error = sqrt(((1.0/originalEvents)*sigmaScaled)*((1.0/originalEvents)*sigmaScaled)+(((-scaledEvents)/(originalEvents*originalEvents))*sigmaOriginal)*(((-scaledEvents)/(originalEvents*originalEvents))*sigmaOriginal));
  
  return error;
}

double findMaximum(double array[], int arrayLength)
{
  double arrayMax = array[0];
  for (int i = 1; i < arrayLength; ++i)
  {
    if (array[i] > arrayMax)
    {
      arrayMax = array[i];
    }
  }
  
  return arrayMax;
}

void createTGraphs(string defaultHist, string scaledUpHist, string scaledDownHist, string scaledUpDownHist, string canvasName, string graphTitle, string xaxis, string yaxis, string legendUp, string legendDown, TFile* histFile, bool needScaledUpDown)
{
  auto defaultPlot = getHist(defaultHist.c_str(), histFile);
  auto scaledUpPlot = getHist(scaledUpHist.c_str(), histFile);
  auto scaledDownPlot = getHist(scaledDownHist.c_str(), histFile);
  auto scaledUpDownPlot = getHist(scaledUpDownHist.c_str(), histFile);

  TAxis* defaultXAxis = defaultPlot->GetXaxis();
  int minMass = defaultXAxis->GetXmin();
  int maxMass = defaultXAxis->GetXmax();
  const int interval = 100;

  const int numBins = (maxMass - minMass) / interval;
  double binMidpointArray[numBins];
  double xErrors[numBins];
  const double binError = (interval * 1.0) / 2;
  int index = 0;  
  for (int lowerBound = minMass; lowerBound < maxMass; lowerBound += interval)
  {
    int upperBound = lowerBound + interval;
    double binMidpoint = calculateMidpoint(lowerBound, upperBound);
    binMidpointArray[index] = binMidpoint;
    index += 1;    
  }
  
  double relDiffArrayUp[numBins];
  double relDiffArrayDown[numBins];
  double relDiffArrayUpDown[numBins];
  double yErrorsUp[numBins];
  double yErrorsDown[numBins];
  double yErrorsUpDown[numBins];

  for (int i = 0; i <= numBins; ++i)
  {
    double numEventsDefault = defaultPlot->GetBinContent(i);
    double numEventsScaledUp = scaledUpPlot->GetBinContent(i);
    double numEventsScaledDown = scaledDownPlot->GetBinContent(i);
    double numEventsScaledUpDown = scaledUpDownPlot->GetBinContent(i);

    double sigmaDefault = defaultPlot->GetBinError(i);
    double sigmaScaledUp = scaledUpPlot->GetBinError(i);
    double sigmaScaledDown = scaledDownPlot->GetBinError(i);
    double sigmaScaledUpDown = scaledUpDownPlot->GetBinError(i);
    
    double relDiffScaledUp = calculateRelativeDifference(numEventsScaledUp, numEventsDefault);
    double relDiffScaledDown = calculateRelativeDifference(numEventsScaledDown, numEventsDefault);
    double relDiffScaledUpDown = calculateRelativeDifference(numEventsScaledUpDown, numEventsDefault);
    
    double yErrorScaledUp = calculateError(numEventsScaledUp, numEventsDefault, sigmaScaledUp, sigmaDefault);
    double yErrorScaledDown = calculateError(numEventsScaledDown, numEventsDefault, sigmaScaledDown, sigmaDefault);
    double yErrorScaledUpDown = calculateError(numEventsScaledUpDown, numEventsDefault, sigmaScaledUpDown, sigmaDefault);
    
    relDiffArrayUp[i-1] = relDiffScaledUp;
    relDiffArrayDown[i-1] = relDiffScaledDown;
    relDiffArrayUpDown[i-1] = relDiffScaledUpDown;
    xErrors[i-1] = binError;
    yErrorsUp[i-1] = yErrorScaledUp;
    yErrorsDown[i-1] = yErrorScaledDown;
    yErrorsUpDown[i-1] = yErrorScaledUpDown;

  }

  if (needScaledUpDown == false)
  {
    TCanvas* c1 = new TCanvas(canvasName.c_str(), canvasName.c_str());
  
    auto scaledUpGraph = new TGraphErrors(numBins, binMidpointArray, relDiffArrayUp, xErrors, yErrorsUp);
    auto scaledDownGraph = new TGraphErrors(numBins, binMidpointArray, relDiffArrayDown, xErrors, yErrorsDown);

    scaledUpGraph->SetTitle(graphTitle.c_str());
    scaledUpGraph->SetLineColor(kRed);
    scaledUpGraph->SetName("up");
 
    scaledDownGraph->SetLineColor(kBlue);
    scaledDownGraph->SetName("down");

    TAxis* xAxisUp = scaledUpGraph->GetXaxis();
    TAxis* yAxisUp = scaledUpGraph->GetYaxis();
    TAxis* xAxisDown = scaledDownGraph->GetXaxis();
    TAxis* yAxisDown = scaledDownGraph->GetYaxis();

    xAxisUp->SetTitle(xaxis.c_str());
    yAxisUp->SetTitle(yaxis.c_str());

    xAxisUp->SetLimits(minMass, maxMass);
    xAxisDown->SetLimits(minMass, maxMass);
  
    double scaledUpMax = findMaximum(relDiffArrayUp, numBins);
    double scaledDownMax = findMaximum(relDiffArrayDown, numBins);
    double graphMax = std::max(scaledUpMax, scaledDownMax);
    scaledUpGraph->SetMaximum(graphMax * 1.1);

    auto fitTGraph = new TF1("fitTGraph", "pol2", minMass, maxMass);
    fitTGraph->SetLineColor(kBlack);
    scaledUpGraph->Fit(fitTGraph, "R");
    
    scaledUpGraph->Draw("AP Z");
    scaledDownGraph->Draw("P Z");   

    const double legendLeftSide = .6;
    auto graphLegend = new TLegend(legendLeftSide, .8, legendLeftSide + .3, .9);
    graphLegend->AddEntry("up", legendUp.c_str(), "L");
    graphLegend->AddEntry("down", legendDown.c_str(), "L");
    graphLegend->Draw();

    c1->Update();
    c1->Write();
    c1->SaveAs((canvasName + ".png").c_str());
  }

  else
  {
    TCanvas* c2 = new TCanvas(canvasName.c_str(), canvasName.c_str());
    auto scaledUpDownGraph = new TGraphErrors(numBins, binMidpointArray, relDiffArrayUpDown, xErrors, yErrorsUpDown);
    
    scaledUpDownGraph->SetTitle(graphTitle.c_str());
    scaledUpDownGraph->SetLineColor(kBlue);
    
    scaledUpDownGraph->GetXaxis()->SetTitle(xaxis.c_str());
    scaledUpDownGraph->GetYaxis()->SetTitle(yaxis.c_str());
    
    scaledUpDownGraph->Draw("AP Z");
    
    c2->Update();
    c2->Write();
    c2->SaveAs((canvasName + ".png").c_str());
  }
}

void plotTwoPlots(string hist1, string hist2, string canvasName, TFile* file1, TFile* file2, string xaxis, string yaxis, string legend1, string legend2, double legendLeftSide)
{
  TCanvas* c1 = new TCanvas(canvasName.c_str(), canvasName.c_str());
  auto plot1 = getHist(hist1.c_str(), file1);
  auto plot2 = getHist(hist2.c_str(), file2);

  c1->SetLogy(true);
  const char* canvasTitle = "Reco Invariant Mass Distribution for LR vs. RL";
  plot1->SetTitle(canvasTitle);

  plot2->Scale(plot1->Integral() / plot2->Integral());

  plot1->SetLineColor(kBlue);
  plot2->SetLineColor(kRed);

  plot1->GetXaxis()->SetTitle(xaxis.c_str());
  plot1->GetYaxis()->SetTitle(yaxis.c_str());

  TAxis* xaxis1 = plot1->GetXaxis();
  TAxis* xaxis2 = plot2->GetXaxis();

  xaxis1->SetLimits(0, 3000);
  xaxis2->SetLimits(0, 3000);

  plot1->Draw("HIST");
  plot2->Draw("SAMEHIST");


  double max = std::max(plot1->GetMaximum(), plot2->GetMaximum());
  plot1->SetMaximum(max * 1.1);

  auto legend = new TLegend(legendLeftSide, .8, legendLeftSide + .3, .9);
  legend->AddEntry(plot1, legend1.c_str(), "L");
  legend->AddEntry(plot2, legend2.c_str(), "L");
  legend->Draw();

  c1->Update();
  c1->Write();
  c1->SaveAs((canvasName + ".png").c_str());
}




