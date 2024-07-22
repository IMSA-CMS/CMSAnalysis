//Intakes a root file created from combinedGenRecoAnalyzer.py and creates divided canvases of histograms based on error type and writes them to a new root file along with the error graphs that were in the original file, which it writes to individual canvases
//Written by: Emily Springer and Abigail VanderPloeg 
//Last Edited: July 18th, 2018

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"



//#include "TTree.h"
//#include "TBrowser.h"
//#include "TH2.h"
//#include "TRandom.h"
#include "TLegend.h"
//#include "TTreeReader.h"
//#include "TTreeReaderValue.h"
//#include "TCanvas.h"
//#include "TVector2.h"
//#include "TPad.h"
//#include "TPaveStats.h"
//#include "TString.h"

std::map<std::string, TGraphErrors*> electronErrorGraphs;
std::map<std::string, TGraphErrors*> muonErrorGraphs;
std::map<std::string, std::vector<Double_t>> electronMaxMin;
std::map<std::string, std::vector<Double_t>> muonMaxMin;

//extracts all the info from given file to be passed and drawn
void setUpVariables(std::string currentLepton, TFile* genOutput);

//creates and adds histograms to the divided canvases
void addToDividedCanvases(std::string currentLepton, int minCut,int maxRange,TFile* outputErrorGraphHists,const char* xAxisTitle,std::map<std::string, TCanvas*> &canvasMap, std::string parameter, std::string barrelState,TFile* genOutput,int canvasIncrement);
//draws each error graph individually 
void drawErrorGraph (std::string errorGraphName, TFile* genOutput);
void drawHistogram(std::string histName, TFile* genOutput);

void createErrorGraph(std::string currentLepton, std::string errorGraphName, vector<double> mean, vector<double> sd, vector<double> meanError, vector<double> sdError, vector<double> massBin, std::string parameter, std::string barrelState);

void drawElectronMuonErrors();

void resolutionRefineOutput()
{
  TFile* genOutput = TFile::Open("electronResolution.root");
  setUpVariables("electron", genOutput);
  genOutput = TFile::Open("muonResolution.root");
  setUpVariables("muon", genOutput);
  drawElectronMuonErrors();
}

void setUpVariables(std::string currentLepton, TFile* genOutput)
{
  //TFile* genOutput = TFile::Open("muonResolution.root");
  TFile* outputErrorGraphHists = TFile::Open("refinedOutputSignFlipMu16.root", "recreate");

  if (genOutput)
  {
    //different loop values for pt vs mass depending on the number of histograms to loop through
    //int ptIncrement = 50;
    //int massIncrement = 100;
    //int maxPtRange=3000;
    //int maxMassRange=4000;
    //int minPtCut=50;
    //int minMassCut=300;

    //Ellyn (6/26/20) - updated values according to current histograms
    int ptIncrement = 50;
    int massIncrement = 100;
    int maxPtRange=1850;
    int maxMassRange=3100;
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
	  //barrelState = barrelState[1];
	  barrelState = barrelStates[a];
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
	  addToDividedCanvases(currentLepton,  minPtCut, maxPtRange, outputErrorGraphHists, xAxisTitle, canvasMap, parameter, barrelState, genOutput, ptIncrement);
	}
	
	if (parameter=="Mass")
	{
	  const char* xAxisTitle="Dilepton Mass (GeV)";
	  addToDividedCanvases(currentLepton, minMassCut, maxMassRange, outputErrorGraphHists, xAxisTitle, canvasMap, parameter, barrelState, genOutput, massIncrement);
	}
      }
    }
	
    

    std::string signFlipsName = "Percent of Sign Flip Matches vs. pT";
    //drawErrorGraph(signFlipsName, genOutput);

    //Ellyn & Eva: seg fault happening right after this line: on drawHistogram line	

    //drawHistogram("histDeltaR",genOutput);

    //Ellyn & Eva: seg fault happens before this line
  }
}

//creates and adds histograms to the divided canvases
void addToDividedCanvases(std::string currentLepton, int minCut,int maxRange,TFile* outputErrorGraphHists,const char* xAxisTitle,std::map<std::string, TCanvas*> &canvasMap, std::string parameter, std::string barrelState,TFile* genOutput,int canvasIncrement)
{
  std::string histsCanvasName;
  int canvasCounter = 1;
  int loopRange = (maxRange-minCut)/canvasIncrement+1;
  //std::cerr << "maxRange = " << maxRange << endl;
  //std::cerr << "minCut = " << minCut << endl;
  //std::cerr << "canvasIncrement = " << canvasIncrement << endl;
  //std::cerr << "loopRange = " << loopRange << endl;
  //loop through the different intervals

  //vectors to hold error data
  std::vector<double> mean;
  std::vector<double> sd;
  std::vector<double> meanError;
  std::vector<double> sdError;
  std::vector<double> massBin;

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

    //Orignial: 
    //std::string histName = "hist" + parameter + "Error" + barrelState + to_string(min) + "to" + to_string(max);

    //Ellyn & Eva (4/1/20):
    //renames the files to match histogram titles in original file
    //std::string histName = "massResolutionHistBin" + to_string(100*x + 300);     

    //Ellyn (6/26/20):
    //names files depending on whether it's pt/mass
    std::string histName;
    if(parameter == "Mass")
    {
      histName = to_string(100*x + 300) + "-" + to_string(100*x + 400) + barrelState + "Mass Resolution Pasted";
    }
    else if(parameter == "Pt")
    {
      histName = to_string(50*x + 50) + "-" + to_string(50*x + 100) + barrelState + "Pt Resolution Pasted";
    }
    else
    {
      std::cerr << "parameter was not of type mass or pt" << endl;
    }
      
    //get the histograms from the root file
    TH1* errorHist=dynamic_cast<TH1*>(genOutput->Get(histName.c_str()));
    //std::cout << "Enters hist loop with combo " + histName <<std::endl;

    if (errorHist && !(errorHist->GetEntries() == 0))
    {
      outputErrorGraphHists->cd();
      errorHist->GetXaxis()->SetTitle(xAxisTitle);
      errorHist->Fit("gaus","","",-0.05,0.05);
      canvasMap[histsCanvasName]->cd(canvasCounter);
      gStyle->SetOptStat("ne");
      gStyle->SetOptFit(0011);
      //errorHist->Draw();

      
      auto function = errorHist->GetFunction("gaus"); //pulls fit function out of hist
      double currentMean = function->GetParameter(1); 
      double currentSD = function->GetParameter(2); 
      double currentMeanError = function->GetParError(1); //had 1 as argument
      double currentSDError = function->GetParError(2); //had 2 as argument

      //adds new data points to vectors
      mean.push_back (currentMean);
      sd.push_back (currentSD);
      meanError.push_back (currentMeanError);
      sdError.push_back (currentSDError);
      if(parameter == "Mass")
      {
        massBin.push_back (100*x + 350);
      }
      else if(parameter == "Pt")
      {
        massBin.push_back (50*x + 50);
      }
      else
      {
        std::cerr << "parameter was not of type mass or pt" << endl;
      }
    }
    else if (errorHist && errorHist->GetEntries() == 0)
    {
      mean.push_back (-1);
      sd.push_back (-1);
      meanError.push_back (1);
      sdError.push_back (1);
      if(parameter == "Mass")
      {
        massBin.push_back (100*x + 350);
      }
      else if(parameter == "Pt")
      {
        massBin.push_back (50*x + 50);
      }
      else
      {
        std::cerr << "parameter was not of type mass or pt" << endl;
      }
    }
    else
    {
      std::cout << "The histogram object for " + barrelState + parameter + " was null!" << std::endl;
    }
    //once there are 8 histograms added or the end of the loop, write and close

    std::cerr << "time out for thee, time out for thee, focus on issues or focus on me" << endl;

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

  std::cerr << parameter + "Error" + barrelState + "ElectronMuonL16\n" << endl;
  //std::cerr << "size of mean vector" << int(mean.size()) << "\n";
  //std::cerr << "elements in mean: ";
  //for(int i=0; i < mean.size(); i++)
  //{
  //  std::cerr << mean.at(i) << " ";
  //}
  //std::cerr << "\n";
  if (!parameter.empty() && !barrelState.empty())
  {
    createErrorGraph(currentLepton, "testing error graph", mean, sd, meanError, sdError, massBin, parameter, barrelState);
  }
	
  //std::cerr << "elements in sd: ";
  //for(int i=0; i < sd.size(); i++)
  //{
  //	std::cerr << sd.at(i) << " ";
  //}
  //std::cerr << "\n";
  //std::cerr << "elements in meanError: ";
  //for(int i=0; i < meanError.size(); i++)
  //{
  //	std::cerr << meanError.at(i) << " ";
  //}
  //std::cerr << "\n";
  //std::cerr << "elements in sdError: ";
  //for(int i=0; i < sdError.size(); i++)
  //{
  //	std::cerr << sdError.at(i) << " ";
  //}
  //std::cerr << "\n";
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

void createErrorGraph(std::string currentLepton, std::string errorGraphName, vector<double> mean, vector<double> sd, vector<double> meanError, vector<double> sdError, vector<double> massBin, std::string parameter, std::string barrelState)
{
  //find max & min y-values of points of both mean and SD vectors
  //same order for all (1. max y-value, 2. min y-value, 3. max error value):
  std::vector<Double_t> meanPointsAndErrors;
  meanPointsAndErrors.push_back (*max_element(mean.begin(), mean.end()));
  meanPointsAndErrors.push_back (*min_element(mean.begin(), mean.end()));
  meanPointsAndErrors.push_back (*max_element(meanError.begin(), meanError.end()));

  std::vector<Double_t> sdPointsAndErrors;
  sdPointsAndErrors.push_back (*max_element(sd.begin(), sd.end()));
  sdPointsAndErrors.push_back (*min_element(sd.begin(), sd.end()));
  sdPointsAndErrors.push_back (*max_element(sdError.begin(), sdError.end()));

  //creating graphs for mean error

  std::string tempName = parameter + " Error Mean " + barrelState + " Electron Muon L16";
  const char* meanTitle = tempName.c_str(); 	
  //auto cTest = new TCanvas(meanTitle,meanTitle,0,0,600,400);
  
  Int_t n = mean.size();
	
  //mass bin
  Double_t* xValues = new double[n];
  for(Int_t i = 0; i < n; i++)
  {
    xValues[i] = massBin[i];
  }	

  //mean of various bins
  Double_t* yValues = new double[n];
  for(Int_t i = 0; i < n; i++)
  {
    yValues[i] = mean[i];
  }

  //"error" in bins
  Double_t* exValues = new double[n];
  for(Int_t i = 0; i < n; i++)
  {
    exValues[i] = 50.0;
  }

  //error in mean
  Double_t* eyValues = new double[n];
  for(Int_t i = 0; i < n; i++)
  {
    eyValues[i] = meanError[i];
  }	

  auto gEMean = new TGraphErrors(n,xValues,yValues,exValues,eyValues);
  gEMean->SetTitle(meanTitle);
  gEMean->SetMarkerStyle(21);

  if (currentLepton == "electron")
  {
    electronErrorGraphs.insert({meanTitle,gEMean});
    electronMaxMin.insert({meanTitle,meanPointsAndErrors});
  }
  else if (currentLepton == "muon")
  {
    muonErrorGraphs.insert({meanTitle,gEMean});
    muonMaxMin.insert({meanTitle,meanPointsAndErrors});
  }
  else
  {
    throw std::runtime_error("particle not of type electron or muon");
  }

  //creating graphs for sd error

  tempName = parameter + " Error SD " + barrelState + " Electron Muon L16";
  const char* sdTitle = tempName.c_str(); 	
  //auto cTest2 = new TCanvas(sdTitle,sdTitle,0,0,600,400);

  //sd of various bins
  for(Int_t i = 0; i < n; ++i)
  {
    yValues[i] = sd[i];
  }

  //error in sd
  for(Int_t i = 0; i < n; ++i)
  {
    eyValues[i] = sdError[i];
  }
  
  auto gESD = new TGraphErrors(n,xValues,yValues,exValues,eyValues);
  gESD->SetTitle(sdTitle);
  gESD->SetMarkerStyle(21);

  if (currentLepton == "electron")
  {
    electronErrorGraphs.insert({sdTitle,gESD});
    electronMaxMin.insert({sdTitle,sdPointsAndErrors});
  }
  else if (currentLepton == "muon")
  {
    muonErrorGraphs.insert({sdTitle,gESD});
    muonMaxMin.insert({sdTitle,sdPointsAndErrors});
  }
  else
  {
    throw std::runtime_error("particle not of type electron or muon");
  }
}

void drawElectronMuonErrors()
{
  auto canvas = new TCanvas("Electron Muon L16 Error Graphs","Electron Muon L16 Error Graphs",1500,750);
  canvas->Divide(4,2);
  int counter = 1;
  for (auto const& eGraph : electronErrorGraphs)
  {
    for (auto const& mGraph : muonErrorGraphs)
    {
      if (eGraph.first == mGraph.first)
      {	
	canvas->cd(counter);
	auto electronGraph = eGraph.second;
	auto muonGraph = mGraph.second;		

	for (auto const& ePoint : electronMaxMin)
	{
	  for (auto const& mPoint : muonMaxMin)
	  {
	    //check if all are referrring to the same graph
	    if (ePoint.first == mPoint.first && mPoint.first == mGraph.first)
	    {
	      //checks if mPoint max y-value is greater than current
	      if (mPoint.second.at(0) > ePoint.second.at(0))
	      {
		eGraph.second->SetMaximum(mPoint.second.at(0)+mPoint.second.at(2));
	      }
	      //checks if mPoint min y-value is less than current
	      if (mPoint.second.at(1) < ePoint.second.at(1))
	      {
		eGraph.second->SetMaximum(mPoint.second.at(1)-mPoint.second.at(2));
	      }
	    }
	  }
	}

	if (eGraph.first.at(0) == 'M')
	{
	  electronGraph->GetXaxis()->SetTitle("Mass (GeV)");
	}
	else if (eGraph.first.at(0) == 'P')
	{
	  electronGraph->GetXaxis()->SetTitle("p_{T} (GeV)");
	}
	else
	{
	  electronGraph->GetXaxis()->SetTitle("GeV");
	}
	electronGraph->GetYaxis()->SetTitle("Error");
	electronGraph->SetLineColor(2);	
	electronGraph->SetMarkerColor(2);
	electronGraph->Draw("ap");
	muonGraph->SetLineColor(4);
	muonGraph->SetMarkerColor(4);
        muonGraph->Draw("p");

	auto legend = new TLegend(0.1,0.78,0.28,0.9);
	legend->AddEntry(electronGraph,"Electron");
	legend->AddEntry(muonGraph,"Muon");
	legend->Draw();

	counter++;
      }
    }
  }
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
