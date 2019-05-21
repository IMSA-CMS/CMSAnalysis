//Intakes a root file created from combinedGenRecoAnalyzer.py and creates divided canvases of histograms based on error type and writes them to a new root file along with the error graphs that were in the original file, which it writes to individual canvases
//Written by: Emily Springer and Abigail VanderPloeg 
//Last Edited: July 18th, 2018

#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TGraph.h"
#include <iostream>
#include <string>
#include <map>
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"

//creates and adds histograms to the divided canvases
void addToDividedCanvases(int minCut,int maxRange,TFile* outputErrorGraphHists,const char* xAxisTitle,std::map<std::string, TCanvas*> &canvasMap, std::string parameter, std::string barrelState,TFile* genOutput,int canvasIncrement);
//draws each error graph individually 
void drawErrorGraph (std::string errorGraphName, TFile* genOutput);
void drawHistogram(std::string histName, TFile* genOutput);

void resolutionRefineOutput(){

  TFile* genOutput = TFile::Open("genOutputE16.root");
  TFile* outputErrorGraphHists = TFile::Open("refinedOutputSignFlipMu16.root", "recreate");
 
  if (genOutput)
    {
      //different loop values for pt vs mass depending on the number of histograms to loop through
      int ptIncrement = 50;
      int massIncrement = 100;

      int maxPtRange=3000;
      int maxMassRange=4000;
      int minPtCut=50;
      int minMassCut=300;

      TGraph* emptyGraph = new TGraph();
      TH1F* emptyHist =  new TH1F();
      
      //map to keep track of the new canvases created
      std::map<std::string, TCanvas*> canvasMap;
      std::string barrelStates [2] = {"BB", "BE"};
      std::string parameters [2] = {"Pt","Mass"};
      std::string errorTypes [2] = {"Mean","Sigma"};

      //loop through the different combinations of barrel states, parameters, and errortypes
      for (unsigned int a=0; a<2; a++)
	{
	  std::string barrelState=barrelStates[a];
	  for (unsigned int b=0; b<2; b++)
	    {
	      std::string parameter=parameters[b];

	      //if Pt, sets barrel state to only the 2nd letter since there is only one particle
	      if (parameter == "Pt")
		{
		  barrelState = barrelState[1];
		}
	      else
		{
		  barrelState = barrelStates[a];
		}

	      //this loop is only for the error graphs, not the histograms
	      for (unsigned int c=0; c<2; c++)
		{
		  std::string errorType=errorTypes[c];

		  std::string errorGraphName = parameter + "Error" + errorType + barrelState;
		  //drawErrorGraph(errorGraphName, genOutput);
		  //sigma/pt graph only
		  if (parameter == "Pt" && errorType=="Sigma")
		    {
		      std::string sigmaDivPtGraphName = string("SigmaDivPt") + barrelState;
		      //drawErrorGraph(sigmaDivPtGraphName, genOutput);
		    }
		}

	      if (parameter=="Pt")
		{
		  const char* xAxisTitle="p_{T} (GeV)";
		  //addToDividedCanvases(minPtCut,maxPtRange,outputErrorGraphHists,xAxisTitle,canvasMap, parameter,barrelState,genOutput,ptIncrement);
		}
	      if (parameter=="Mass")
		{
		  const char* xAxisTitle="Dilepton Mass (GeV)";
		  //addToDividedCanvases(minMassCut,maxMassRange,outputErrorGraphHists,xAxisTitle,canvasMap, parameter,barrelState,genOutput,massIncrement);
		}
	    }
	}
      std::string signFlipsName = "Percent of Sign Flip Matches vs. pT";
      //drawErrorGraph(signFlipsName, genOutput);
      drawHistogram("histDeltaR",genOutput);
    }
}

//creates and adds histograms to the divided canvases
void addToDividedCanvases(int minCut,int maxRange,TFile* outputErrorGraphHists,const char* xAxisTitle,std::map<std::string, TCanvas*> &canvasMap, std::string parameter, std::string barrelState,TFile* genOutput,int canvasIncrement)
{
  std::string histsCanvasName;
  int canvasCounter = 1;
  int loopRange = (maxRange-minCut)/canvasIncrement+1;
  //loop through the different intervals
  for (int x=0; x<loopRange; x++)
    {
      int min=minCut + canvasIncrement*(x);
      int max=min+canvasIncrement;
      
      //create a new canvas and divide on first loop through or after recently closing the last one
      if (canvasCounter==1)
	{
	  //8 histograms per canvas
	  int canvasMax = min + canvasIncrement*8;
	  histsCanvasName = "hists" + parameter + "Error" + barrelState + to_string(min) + "to" + to_string(canvasMax);
	  canvasMap[histsCanvasName] = new TCanvas(histsCanvasName.c_str(), histsCanvasName.c_str(), 400, 400);
	  canvasMap[histsCanvasName]->Divide(4, 2);
	}
      //follows naming convention that is in the inputted root file
      std::string histName = "hist" + parameter + "Error" + barrelState + to_string(min) + "to" + to_string(max);
      //get the histograms from the root file
      TH1* errorHist=dynamic_cast<TH1*>(genOutput->Get(histName.c_str()));
      //std::cout << "Enters hist loop with combo " + histName <<std::endl;
      if (errorHist)
	{
	  outputErrorGraphHists->cd();
	  errorHist->GetXaxis()->SetTitle(xAxisTitle);
	  canvasMap[histsCanvasName]->cd(canvasCounter);
	  gStyle->SetOptStat("ne");
	  gStyle->SetOptFit(0011);
	  errorHist->Draw();
	}
      else
	{
	  std::cout << "The histogram object for " + barrelState + parameter + " was null!" << std::endl;
	}
      //once there are 8 histograms added or the end of the loop, write and close
      if (canvasCounter==8 || max==maxRange)
	{		  			 
	  gStyle->SetOptStat("ne");
	  gStyle->SetOptFit(0011);
	  canvasMap[histsCanvasName]->Write();
	  canvasMap[histsCanvasName]->Close();
	  canvasCounter=0;
	}
      canvasCounter++;
    }
}

void drawHistogram(std::string histName, TFile* genOutput)
{
  TH1* errorHist=dynamic_cast<TH1*>(genOutput->Get(histName.c_str()));
  if (!errorHist)
    {
      std::cout<<"file not found!"<<std::endl; 
    }
  std::string histTitle = "Delta R";
  TCanvas* histogramCanvas=new TCanvas(histTitle.c_str(), histTitle.c_str(), 500,500);
  errorHist->GetXaxis()->SetTitle("Delta R");
  errorHist->GetYaxis()->SetTitle("Number of Events");
  errorHist->SetTitle("Muon Delta R Lambda 16 TeV");
  gStyle->SetOptStat(0);
  errorHist->Draw();
  histogramCanvas->Write();
  histogramCanvas->Close();

}


void drawErrorGraph(std::string errorGraphName, TFile* genOutput)
{
  //get the error graph from the inputed genOutput root file
  TGraph* errorGraph=dynamic_cast<TGraph*>(genOutput->Get(errorGraphName.c_str()));
  TCanvas* errorGraphCanvas=new TCanvas(errorGraphName.c_str(), errorGraphName.c_str(), 500, 500);
  errorGraphCanvas->cd();

  //supposed to remove points at the end that are 0, but it does not currently work
  if (errorGraphName == "Percent of Sign Flip Matches vs. pT")
    {
      double x = 0;
      double y = 0;
      int index = errorGraph->GetN()-1; //index begins with last point on the graph
      errorGraph->GetPoint(index,x,y);
      while (y==0)
	{
	  std::cout<<to_string(index)+" "+to_string(x)+" "+to_string(y)<<std::endl;
	  errorGraph->RemovePoint(index);
	  index-=1;
	  errorGraph->GetPoint(index,x,y);
	}
      errorGraph->GetXaxis()->SetTitle("p_{T} (GeV)");
      errorGraph->GetYaxis()->SetTitle("Fraction of Sign Flips");
    }

  //checks if the graph exists
  if (errorGraph)
    {
      errorGraph->SetMarkerStyle(21);
      errorGraph->SetMarkerSize(0.5);
      errorGraph->SetMarkerColor(2);
      errorGraph->Draw("AP");
      gStyle->SetOptStat("ne");
      gStyle->SetOptFit(0011);   
      errorGraphCanvas->Write();
      errorGraphCanvas->Close();
    }
  else
    {
      std::cout<<errorGraphName+" is not found"<<std::endl;
    }
  
}
