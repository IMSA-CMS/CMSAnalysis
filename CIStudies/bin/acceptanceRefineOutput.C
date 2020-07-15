//Acceptance Migration Overlay Histogram Creator 
//John Woods and Matt Hokinson - July 6th, 2018 
//This will read two root files to compare Reco EE and MUMU data 

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
//ROOT Components to include (Not sure if all are needed) (oof)
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TH1.h"
#include "TLegend.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TCanvas.h"
#include "TVector2.h"
#include "TPad.h"
#include "TPaveStats.h"
#include "TString.h"

//using std::string;
//using std::cout;

//Combining the histograms that are submitted to the fuction into a single canvas
void createCombinedCanvas(TH1* hist1, TH1* hist2, TCanvas* c, int loopCount);

//Create the combined Acceptance Graphs and puts it in the output file 
TCanvas* createAcceptanceCombined(TH1* hist1, TH1* hist2);

//Find which pad to put the next canvas on
int getPad(int loopNumber, int padsDivisor);

int const padsPerCanvas = 8;

void acceptanceRefineOutput()// electronMuonOverlayer()
{
	std::cerr << "donut" << endl;        
	int numberOfHists = 28;//Number of Bins
  	string const inputFileName = "electron_16TeV_AccMig_MassRes.root";
  	string const inputFileName2 = "muon_16TeV_AccMig_MassRes.root";
  	string const canvasOutputName = "Muon_Electron_16TeV_Overlayed.root";
  
  	//Here we are going to create arrays of muon and electron histograms  
  	TH1* muonHists[numberOfHists];
  	TH1* elecHists[numberOfHists];

	TH1* acceptanceFile1;
	TH1* acceptanceFile2;
  
  	//The file name is hard coded in, and changing it would mean you also have to change the names of the hists that you use to fill in the arrays 
  	TFile* inputFile = new TFile(inputFileName.c_str());
  
  	//This is going to loop throught the root file and set the hist arrays 
  	for (int i = 0; i < numberOfHists; ++i)
    {
      	//These two have to be hardcodes in anc changed when running with different files - (Use TBrowser to see hist names)
      	
	//Original:
	//string mH = "recoHistBin" + std::to_string(i);
	
	//Modified (Ellyn, 3/26/20):	
	string mH = "recoHistBin" + std::to_string(100*i + 300);
	//starts at 300 and adds 100 each time

	//Original:
	//string accH = "Total Reco Matched hist";

	//Modified (Ellyn, 3/26/20):
	string accH = "Reco Invariant Mass Pasted";

	//Ellyn: muonHists[i] is null --> not being filled properly?
        inputFile->GetObject(mH.c_str(), muonHists[i]);
	if (!muonHists[i])
	{
		throw std::runtime_error("out of red bean buns");	
	}
	inputFile->GetObject(accH.c_str(), acceptanceFile1);
	if (!acceptanceFile1)
	{
		throw std::runtime_error("milk tea cooled");
	}
    }
	
	std::cerr << "crepe" << endl;	
  
  	//The name of the second file 
  	TFile* inputFileTwo = new TFile(inputFileName2.c_str());
  
  	//This is the second loop throught the other file 
  	for (int i = 0; i < numberOfHists; ++i)
    {
      	string elH = "recoHistBin" + std::to_string(100*i + 300);

	string accH = "Reco Invariant Mass Pasted";

        inputFileTwo->GetObject(elH.c_str(), elecHists[i]);
	inputFileTwo->GetObject(accH.c_str(), acceptanceFile2);
    }
  
	std::cerr << "tomato and egg noodles" << endl;		

  	//Create an array of Canvases to pass into the create combined canvas funciton 
  	TCanvas* canvases[static_cast<int>(ceil(numberOfHists / 8)) + 1];//We put a plus one because for some reason the last Canvas was not written to the ROOT file 
  	
  	//This will loop through the hists to 1) Create a new canvas 2) Create new overlay hist 3) Put these into the new output TFile 
  	for( int i = 0; i < numberOfHists; ++i)
    {
      if((i % padsPerCanvas) == 0)
      {
        string const canvasName = "Muon_VS_Electron_Migration" + std::to_string(i / padsPerCanvas);
        string const canvasTitle = "Muon Acceptance * Migration " + std::to_string((i / padsPerCanvas) + 1);
      	canvases[i / padsPerCanvas] = new TCanvas(canvasName.c_str(), canvasTitle.c_str());
        canvases[i / padsPerCanvas]->Divide(2,4);//We are able to do i / 8 here becuase we know i is divisible by 8 already 
      }
      
	std::cerr << "fried rice" << endl;

	//Ellyn: seg faults between fried rice and hotpot

	if (!elecHists[i])
	{
		throw std::runtime_error("tried eating ice cream with chopsticks");	
	}		

      //cout << "Using index " << static_cast<int>(ceil(i / padsPerCanvas)) << "\n";
      createCombinedCanvas(muonHists[i], elecHists[i], canvases[static_cast<int>(ceil(i / padsPerCanvas))], i);//The object will default write to the last opened TFile, 
     
	std::cerr << "hotpot" << endl; 															  //which is outputFile
    } 

                                       
    //Creating the Output ROOT File
  	TFile* canvasOutput = new TFile(canvasOutputName.c_str(), "RECREATE");
        
  	//Write the canvases into the output ROOT file 
    for(TCanvas* canvas : canvases)
    {
       canvas->Write();                                  
    }

    createAcceptanceCombined(acceptanceFile1, acceptanceFile2)->Write();
                                  
}

//Canvas creator - Takes in two histograms and returns a TCanvas* that contains both histograms and a legend as a key 
//The canvasName is what shows up in the TBrowser (ie. what is used to find the canvas when extracting them from the output root file)
void createCombinedCanvas(TH1* hist1, TH1* hist2, TCanvas* c, int loopCount)
{
  std::cerr << "mochi" << endl;
  //cout << getPad(loopCount, padsPerCanvas) << " Pad number we are doing into\n";
  c->cd(getPad(loopCount, padsPerCanvas));
   
  std::cerr << "egg tart" << endl;

  int  max2 = hist2->GetMaximum();

  std::cerr << "rice cracker" << endl;

  int  max1 = hist1->GetMaximum();//Checks the bin with the highest number of events 
  
  //Ellyn: seg faults before seaweed
  std::cerr << "seaweed" << endl;

  double scaleFactor = (hist1->Integral())/(hist2->Integral());//Uses integrals to find the scale factor - works to scale up and down 
  //It doesn't matter that we change the values because we are only looking at the percentages for the Acceptance * Migration on RECO 

  //Ellyn: seg faults before snow pear
  std::cerr << "snow pear" << endl;
  //MAYBE DELETE THIS LATER 
  //if (max > muonMax)
  //{
      //hist1->SetAxisRange(0., (max + 5),"Y");
  //}
  if((hist2->Integral()) < (hist1->Integral()))//Checks if hist2 will be scaled up 
    {
      hist1->SetAxisRange(0., ((max2 * scaleFactor) + 10),"Y");
    }
  if(((hist2->Integral()) >= (hist1->Integral())) && (max2 > max1))//Checks the same thing 
    {
      hist1->SetAxisRange(0., ((max2 * scaleFactor) + 10),"Y");
    }
  //We used two if statements to make the code easier to read 

  hist1->SetTitle("Acceptance * Migration: Muons and Electrons at #Lambda = 16 TeV");
  hist1->SetLineWidth(2);
  hist1->SetLineStyle(1);
  hist1->GetXaxis()->SetTitle("Invarient Mass (GeV)");
  hist1->GetYaxis()->SetTitle("Number of Events");
  hist1->SetStats(kFALSE);//This gets rid of the default legend states
  hist1->Draw();

  hist2->Scale(scaleFactor);
  hist2->SetLineColor(2);
  hist2->SetLineWidth(2);	
  hist2->SetLineStyle(1);
  hist2->GetXaxis()->SetTitle("Invarient Mass (GeV)");
  hist2->GetYaxis()->SetTitle("Number of Events");
  hist2->SetStats(kFALSE);//This gets rid of the default legend stats
  hist2->Draw("Same");
  
  //Ellyn: seg faults before hot cheetos
  std::cerr << "hot cheetos" << endl;

  auto legend = new TLegend(.78,.76,.98,.86);
  legend->AddEntry(hist2,"Scaled to other Hist","l");
  string const legendEntry = "Mass Bin: " + std::to_string((loopCount * 100) + 300);
  legend->AddEntry((TObject*)0, legendEntry.c_str(), "");
  legend->Draw();

  auto legend2 = new TLegend(.78,.65,.98,.75);
  //Original:
  //legend2->AddEntry(hist1,"#Lambda = 1 TeV","l");
  //Ellyn & Eva (4/8/20)
  legend2->AddEntry(hist1,"Electron Reco","l");
  //Original:
  //legend2->AddEntry(hist2,"#Lambda = 16 TeV","l");
  //Ellyn & Eva (4/8/20)
  legend2->AddEntry(hist2,"Muon Reco","l");
  legend2->Draw();
}

TCanvas* createAcceptanceCombined(TH1* hist1, TH1* hist2)
{
  TCanvas* c = new TCanvas("AcceptanceOverlay", "Acceptance Overlay");
  for (int i = 1; i < ((hist1 -> GetNbinsX()) - 2); ++i)
    {
      std::cout << std::to_string(200 + (i * 100)) + "-" + std::to_string(300 + (i * 100)) + "\t" + std::to_string(hist1 -> GetBinContent(i)) + "\n";
    }
  hist1->SetTitle("Acceptance for Muons and Electrons at #Lambda = 16 TeV");
  hist1->SetLineWidth(2);
  hist1->SetLineStyle(1);
  hist1->GetYaxis()->SetTitle("Percent of Accepted Events");
  hist1->GetXaxis()->SetTitle("Mass Bin");
  hist1->SetStats(kFALSE);//This gets rid of the default legend states
  hist1->Draw();

  hist2->SetLineColor(2);
  hist2->SetLineWidth(2);	
  hist2->SetLineStyle(1);
  hist2->GetYaxis()->SetTitle("Percent of Accepted Events");
  hist2->GetXaxis()->SetTitle("Mass Bin");
  hist2->SetStats(kFALSE);//This gets rid of the default legend stats
  hist2->Draw("Same");

  auto legend2 = new TLegend(.78,.65,.98,.75);
  legend2->AddEntry(hist1,"Electrons","l");
  legend2->AddEntry(hist2,"Muons","l");
  legend2->Draw();

  return c;
}

int getPad(int loopNumber, int padsDivisor)
{
  return (loopNumber % padsDivisor) + 1;
}


