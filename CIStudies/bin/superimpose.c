//
// Original Author:  Teodor Tchalakov
//            User:  ttchalak
//         Created:  Mon, 20 Jul 2020
//
//
#include <algorithm>
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include <iostream>
#include <string>
#include <vector>
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include <new>


TH1F* getHist(std::string name, TFile* file);

struct Plot
{
  TFile *file;
  TH1F *plot;
  string legend;
  string drawArgs = "";

  Plot(string filePath, string histPath, EColor color, string ilegend, string idrawArgs)
  {
    file = TFile::Open(filePath.c_str());
    plot = getHist(histPath.c_str(), file);
    plot->SetLineColor(color);
    legend = ilegend;
    drawArgs = "9 " + idrawArgs;
  }

  Plot(string filePath, string histPath, EColor color, string ilegend)
  {
    file = TFile::Open(filePath.c_str());
    plot = getHist(histPath.c_str(), file);
    plot->SetLineColor(color);
    legend = ilegend;
    drawArgs = "9";
  }

  Plot(string filePath, string histPath, string ilegend, string idrawArgs)
  {
    file = TFile::Open(filePath.c_str());
    plot = getHist(histPath.c_str(), file);
    legend = ilegend;
    drawArgs = "9 PLC PMC " + idrawArgs;
  }

  Plot(string filePath, string histPath, string ilegend)
  {
    file = TFile::Open(filePath.c_str());
    plot = getHist(histPath.c_str(), file);
    legend = ilegend;
    drawArgs = "9 PLC PMC";
  }

  Plot()//You get errors for some reason if there is not a default do not use this one
  {
    file = nullptr;
    plot = nullptr;
    legend = "default";
    drawArgs = " ";
  }
};

struct Image
{
  vector<Plot> histograms;
  double legendLeftSide = .4;
  string xAxis;
  string yAxis;
  string canvasName;
  EColorPalette palette = kOcean;

  Image(double legendLeftSide, string xAxis, string yAxis, string canvasName, EColorPalette palette, vector<Plot> histograms)
  {
    this->legendLeftSide = legendLeftSide;
    this->histograms = histograms;
    this->xAxis = xAxis;
    this->yAxis = yAxis;
    this->canvasName = canvasName;
    this->palette = palette;
  }

  Image(double legendLeftSide, string xAxis, string yAxis, string canvasName, vector<Plot> histograms)
  {
    this->legendLeftSide = legendLeftSide;
    this->histograms = histograms;
    this->xAxis = xAxis;
    this->yAxis = yAxis;
    this->canvasName = canvasName;
  }
};

void superimpose();
void superimpose(vector<Image> histConfigurations);
int getLargestIndex(vector<Plot> histograms);
void scaleToHighest(vector<Plot> histograms);
void superImposePlots(Image configuration);

/*
  To use this root script setup paths as strings to the root files
  and the paths in the root files to the histograms that you want.
  Then either programatically iterate to configure a vector of Image
  structs that house a configuration for each picture that you will
  in the end. Every Image will have a vector of Plots which represent
  the histograms that will be superimposed over each other and their
  own configurations.
 */
void superimpose(){
  string file1 = "leftHiggsVEV5Analyzed.root";
  string file2 = "rightHiggsVEV5Analyzed.root";

  string file3 = "leftHiggsVEV5Mass300GeVAnalyzed.root";
  string file4 = "leftHiggsVEV5Mass800GeVAnalyzed.root";
  string file5 = "leftHiggsVEV5Mass1300GeVAnalyzed.root";

  string file6 = "leftHiggsVEV0Mass200SameFlavorPairsAnalyzed.root";

  string file7 = "leftHiggsVEV0Mass200SameFlavorPairsQuadLeptonAnalyzed.root";
  string file8 = "leftHiggsVEV0Mass300SameFlavorPairsQuadLeptonAnalyzed.root";
  string file9 = "leftHiggsVEV0Mass500SameFlavorPairsQuadLeptonAnalyzed.root";
  string file10 = "leftHiggsVEV0Mass800SameFlavorPairsQuadLeptonAnalyzed.root";
  string file11 = "leftHiggsVEV0Mass1300SameFlavorPairsQuadLeptonAnalyzed.root";

  string file12 = "leftHiggsVEV0Mass200SameFlavorPairsAnalyzed.root";
  string file13 = "leftHiggsVEV0Mass300SameFlavorPairsAnalyzed.root";
  string file14 = "leftHiggsVEV0Mass500SameFlavorPairsAnalyzed.root";
  string file15 = "leftHiggsVEV0Mass800SameFlavorPairsAnalyzed.root";
  string file16 = "leftHiggsVEV0Mass1300SameFlavorPairsAnalyzed.root";

  string file17 = "zzOutput.root"; //Higgs to WW background
  string file18 = "dyOutput.root";
  string file19 = "ttbarOutput.root";
  string file20 = "Output.root";
  string file21 = "TTOutput.root";


  string hist1 = "Dimuon/Muon+ pt High";
  string hist2 = "Dimuon/Muon+ pt Low";

  string hist3 = "Dimuon/Invariant mass";
  string hist4 = "Dimuon/Four Lepton Invariant mass";

  string hist5 = "Dimuon/Leading Lepton pT";
  string hist6 = "Dimuon/Second Leading Lepton pT";
  string hist10 = "Dimuon/Third Leading Lepton pT";
  string hist11 = "Dimuon/Fourth Leading Lepton pT";

  string hist7 = "Dimuon/Electron+ eta";
  string hist12 = "Dimuon/delta r";

  string hist8 = "Quadlepton/Invariant mass";
  string hist9 = "Quadlepton/Invariant mass OSDL";

  string hist13 = "Missing Et"; //Higgs to WW background
  string hist14 = "Reco Same Sign Invariant Mass"; //Higgs to WW background
  string hist15 = "Reco Opposite Sign Invariant Mass"; //Higgs to WW background
  string hist16 = "Transverse Momentum"; //Higgs to WW background

  string hist17 = "Dimuon/Missing Et"; //Higgs to WW background

  vector<Image> images
    {
        Image(.4, "GeV", "Events", "Missing Et", kCool
            ,vector<Plot> {
                Plot(file17, hist13, kRed, "ZZ Background")
                ,Plot(file18, hist13, kBlue, "Drell-Yan Background")
                ,Plot(file19, hist13, kGreen, "TTBar Background")
                ,Plot(file21, hist13, kBlack, "Higgs to Tau Tau")
                }) 
        ,Image(.4, "GeV", "Events", "Reco Same Sign Invariant Mass", kCool
            ,vector<Plot> {
                Plot(file17, hist14, kRed, "ZZ Background")
                ,Plot(file18, hist14, kBlue, "Drell-Yan Background")
                ,Plot(file19, hist14, kGreen, "TTBar Background")
                ,Plot(file21, hist14, kBlack, "Higgs to Tau Tau")
                })
        ,Image(.4, "GeV", "Events", "Reco Opposite Sign Invariant Mass", kCool
            ,vector<Plot> {
                Plot(file17, hist15, kRed, "ZZ Background")
                ,Plot(file18, hist15, kBlue, "Drell-Yan Background")
                ,Plot(file19, hist15, kGreen, "TTBar Background")
                ,Plot(file21, hist15, kBlack, "Higgs to TT")
                }) 
        ,Image(.4, "GeV", "Events", "Transverse Momentum", kCool
            ,vector<Plot> {
                Plot(file17, hist16, kRed, "ZZ Background")
                ,Plot(file18, hist16, kBlue, "Drell-Yan Background")
                ,Plot(file19, hist16, kGreen, "TTBar Background")
                ,Plot(file21, hist16, kBlack, "Higgs to Tau Tau")
                })
        /*Image(.4, "GeV", "Events", "Reconstructed SSDL Doubly Charged Higgs Mass", kCool
                ,vector<Plot> {
                Plot(file7, hist8, kRed, "200 GeV Boson")
                    ,Plot(file8, hist8, kBlue, "300 GeV Boson")
                    ,Plot(file9, hist8, kGreen, "500 GeV Boson")
                    ,Plot(file10, hist8, kOrange, "800 GeV Boson")
                    ,Plot(file11, hist8, kBlack, "1300 GeV Boson")
                    })
        ,Image(.4, "GeV", "Events", "Reconstructed OSDL Doubly Charged Higgs Mass", kCool
              ,vector<Plot> {
                Plot(file7, hist9, kRed, "200 GeV Boson")
                  ,Plot(file8, hist9, kBlue, "300 GeV Boson")
                  ,Plot(file9, hist9, kGreen, "500 GeV Boson")
                  ,Plot(file10, hist9, kOrange, "800 GeV Boson")
                  ,Plot(file11, hist9, kBlack, "1300 GeV Boson")
                  })
        ,Image(.4, "pT GeV", "Events", "Sorted Lepton pT GeV for 200 GeV Higgs", kCool
               ,vector<Plot> {
                 Plot(file6, hist5, kBlack, "Leading Lepton pT")
                   ,Plot(file6, hist6, kRed, "Second Leading Lepton pT")
                   ,Plot(file6, hist10, kBlue, "Third Leading Lepton pT")
                   ,Plot(file6, hist11, kGreen, "Fourth Leading Lepton pT")
                   })
        ,Image(.4, " ", "Events", "Eta", kOcean
              ,vector<Plot> {
                 Plot(file6, hist7, kRed, "200 GeV Higgs")
                   ,Plot(file4, hist7, kBlue, "800 GeV Higgs")
                   ,Plot(file5, hist7, kGreen, "1300 GeV Higgs")
                   })
        ,Image(.4, " ", "Events", "delta r", kOcean
               ,vector<Plot> {
                 Plot(file12, hist12, kRed, "200 GeV Higgs")
                   ,Plot(file15, hist12, kBlue, "800 GeV Higgs")
                   ,Plot(file16, hist12, kGreen, "1300 GeV Higgs")
                   })
  
      Image(.4, "Leading Lepton pT GeV", "Events", "Leading Lepton pT", kCool
            ,vector<Plot> {
              Plot(file6, hist5, kBlue, "200 GeV Boson")
                })
        ,Image(.4, "Reconstructed SSDL Doubly Charged Higgs Mass GeV", "Events", "Reconstructed SSDL Doubly Charged Higgs Mass", kCool
              ,vector<Plot> {
                Plot(file7, hist8, kBlue, "200 GeV Boson")
                  })
        ,Image(.4, "Lepton GeV", "Events", "Leading pt", kDarkRainBow
               ,std::vector<Plot> {
                 Plot(file3, hist5, kRed, "300 GeV Higgs")
                   ,Plot(file4, hist5, kBlue, "800 GeV Higgs")
                   ,Plot(file5, hist5, kGreen, "1300 GeV Higgs")
                   })
        ,Image(.4, "Four Lepton Invariant mass GeV", "Events", "Four Lepton Masses", kCool
              ,vector<Plot> {
                Plot(file3, hist4, "300 GeV Higgs")
                  ,Plot(file4, hist4, "800 GeV Higgs")
                  ,Plot(file5, hist4, "1300 GeV Higgs")
                  })
        ,Image(.4, "GeV", "Events", "Leading and Second Highest pT Lepton", kCool
              ,vector<Plot> {
                Plot(file3, hist5, kRed, "300 Leading")
                  ,Plot(file4, hist5, kBlue, "800 Leading")
                  ,Plot(file5, hist5, kGreen, "1300 Leading")
                  ,Plot(file3, hist6, kRed, "300 Second Highest")
                  ,Plot(file4, hist6, kBlue, "800 Second Highest")
                  ,Plot(file5, hist6, kGreen, "1300 Second Highest")
                  })
        ,Image(.4, "GeV", "Events", "Leading and Second Highest pT Lepton 200 GeV", kCool
              ,vector<Plot> {
                 Plot(file1, hist5, kBlack, "Leading")
                   ,Plot(file1, hist6, kRed, "Second Highest")
                  })
        ,Image(.4, " ", "Events", "Eta for 300_800_1300", kOcean
              ,vector<Plot> {
                 Plot(file3, hist7, kRed, "300 GeV Higgs")
                   ,Plot(file4, hist7, kBlue, "800 GeV Higgs")
                   ,Plot(file5, hist7, kGreen, "1300 GeV Higgs")
                  })*/
        };
  /* EXAMPLES
  vector<Image> images
    {
      Image(.4, "GeV", "Events", "LeftHiggs Invariant Mass", kDarkRainBow
            ,std::vector<Plot> {
              Plot(file3, hist3, kRed, "300")
                ,Plot(file4, hist3, kBlue, "800")
                ,Plot(file5, hist3, kGreen, "1300")
                })
        ,Image(.4, "GeV", "Events", "LeftHiggs Invariant Mass2", kPigeon
              ,vector<Plot> {
                Plot(file3, hist3, "300")
                  ,Plot(file4, hist3, "800")
                  ,Plot(file5, hist3, "1300")
                  })
        ,Image(.4, "GeV", "Events", "LeftHiggs Invariant Mass3", kCool
              ,vector<Plot> {
                Plot(file3, hist3, "300")
                  ,Plot(file4, hist3, kBlack, "800")
                  ,Plot(file5, hist3, "1300")
                  })
        };
  */ 
  
  superimpose(images);
}

void superimpose(vector<Image> histConfigurations)
{
  for(int i = 0; i < (int) histConfigurations.size(); i++)
    {
      scaleToHighest(histConfigurations[i].histograms);
      superImposePlots(histConfigurations[i]);
    }
}

TH1F* getHist(std::string name, TFile* file)
{
  auto hist = dynamic_cast<TH1F*>(file->Get(name.c_str()));
  if (!hist)
    throw std::runtime_error(name + " not found!");
  return hist;
}

int getLargestIndex(vector<Plot> histograms)
{
  int largestIndex = 0;
  for(int i = 0; i < (int) histograms.size(); i++)
    {
      if(histograms[largestIndex].plot->GetMaximum() < histograms[i].plot->GetMaximum())
        {
          largestIndex = i;
        }
    }
  return largestIndex;
}

void scaleToHighest(vector<Plot> histograms)
{
  int largestIndex = getLargestIndex(histograms);

  for(int i = 0; i < (int) histograms.size(); i++)
    {
      histograms[i].plot->Scale(histograms[largestIndex].plot->Integral() / histograms[i].plot->Integral());
    }
}

void superImposePlots(Image config)
{
  TCanvas* c1 = new TCanvas();
  c1->SetWindowSize(1920, 1080);
  auto legend = new TLegend(config.legendLeftSide, .8, config.legendLeftSide + .3, .9);

  gStyle->SetPalette(config.palette);

  double max = config.histograms[getLargestIndex(config.histograms)].plot->GetMaximum();
  config.histograms[0].plot->SetMaximum(max * 1.1);
  std::cout << "For the " << config.canvasName << " histogram, the maximum is from " << config.histograms[getLargestIndex(config.histograms)].legend << " at " << max << std::endl;

  config.histograms[0].plot->GetXaxis()->SetTitle(config.xAxis.c_str());
  config.histograms[0].plot->GetYaxis()->SetTitle(config.yAxis.c_str());
  config.histograms[0].plot->SetTitle(config.canvasName.c_str());

  for(int i = 0; i < (int) config.histograms.size(); i++)
    {
      if(i == 0) config.histograms[i].plot->Draw(("HIST" + config.histograms[i].drawArgs).c_str());
      else config.histograms[i].plot->Draw(("SAME HIST" + config.histograms[i].drawArgs).c_str());
      legend->AddEntry(config.histograms[i].plot, config.histograms[i].legend.c_str(), "L");
    }

  legend->Draw();

  c1->Update();
  c1->SaveAs((config.canvasName + ".png").c_str());

  //Uncomment to have Canvas automatically close after run
  //delete c1;
  //delete legend;
}