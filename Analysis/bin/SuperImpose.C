#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Analysis/interface/Estimator.hh"
#include "CMSAnalysis/Analysis/interface/FitEstimator.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include <fstream>
#include "THStack.h"
#include "TString.h"
#include <iostream>
#include <memory>
#include "TCanvas.h"
#include "TSystem.h"
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TStyle.h"

void SuperImpose() {
  // Change extraText here
  auto plotFormatter = std::make_shared<PlotFormatter>(false, "Simulation Preliminary");

  // Change the filePath here. This should be the longest branch all input files have in common.
  //const std::string filePath = "/uscms/home/ceddingt/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/";
  const std::string filePath = "/uscms/home/dlimosne/analysis/CMSSW_14_0_1/src/CMSAnalysis/Output/";
  // Write the remaining file paths and graphs here. The hist in index 0 of the hists vector gets pulled from the file at index 0 in files, and so on.
  // Write your graph names here (for the legend)
  // std::vector<std::string> files = {"scalefactors.root", "noscalefactors.root"};
  // std::vector<std::string> hists = {
  //     "eeuu__hists/eeuu_Reco Same Sign Invariant Mass",
  //     "eeuu__hists/eeuu_Reco Same Sign Invariant Mass"
  // };
  // std::vector<TString> names = {"Scale Factor", "No Scale Factor"};
  // std::vector<int> colors = {kBlack, 2, 6};

  // TString xTitle = "Invariant Mass (GeV)";
  // TString yTitle = "Event Ratio";

  // darkphoton_DPMass0_2-9 super imposed graphs
  // std::vector<std::string> files = {"darkPhoton_DpMass0_2.root", "darkPhoton_DpMass0_4.root", "darkPhoton_DpMass0_6.root", "darkPhoton_DpMass0_9.root"};
  // std::vector<std::string> hists = {
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput"
  // };
  
  // std::vector<TString> names = {"darkPhoton_DpMass0_2", "darkPhoton_DpMass0_4", "darkPhoton_DpMass0_6", "darkPhoton_DpMass0_9" };
  // std::vector<int> colors = {kBlack, kBlue, kGreen, kRed};

  // TString xTitle = "BDT Output";
  // TString yTitle = "Number of Events";


  // darkPhoton_DpMass(1_2 - 4_0) super imposed graphs
  // std::vector<std::string> files = {"darkPhoton_DpMass1_2.root", "darkPhoton_DpMass1_5.root", "darkPhoton_DpMass2_5.root", "darkPhoton_DpMass4_0.root"};
  // std::vector<std::string> hists = {
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput"
  // };
  
  // std::vector<TString> names = {"darkPhoton_DpMass1_2", "darkPhoton_DpMass1_5", "darkPhoton_DpMass2_5", "darkPhoton_DpMass4_0" };
  // std::vector<int> colors = {kBlack, kBlue, kGreen, kRed};

  // TString xTitle = "BDT Output";
  // TString yTitle = "Number of Events";



  // darkPhotonDecay super imposed graphs
  // std::vector<std::string> files = {"darkPhotonDecay_Higgs4DP.root", "darkPhotonDecay_HiggsDPZ.root", "darkPhotonDecay_SUSY.root", "darkPhotonDecay_ZPrime.root"};
  // std::vector<std::string> hists = {
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput"
  // };
  
  // std::vector<TString> names = {"darkPhotonDecay_Higgs4DP", "darkPhotonDecay_HiggsDPZ", "darkPhotonDecay_SUSY", "darkPhotonDecay_ZPrime" };
  // std::vector<int> colors = {kBlack, kBlue, kGreen, kRed};

  // TString xTitle = "BDT Output";
  // TString yTitle = "Number of Events";



  // darkPhotonHiggs super imposed graphs - Note that for some reason the root files are empty for these two files
  std::vector<std::string> files = {"darkPhotonHiggs125.root", "darkPhotonHiggs300.root"};
  std::vector<std::string> hists = {
      "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
      "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput"
  };
  
  std::vector<TString> names = {"darkPhotonHiggs125", "darkPhotonHiggs300"};
  std::vector<int> colors = {kBlack, kBlue};

  TString xTitle = "BDT Output";
  TString yTitle = "Number of Events";



  // darkPhotonRun2FSR super imposed graphs
  // std::vector<std::string> files = {"darkPhotonRun2FSR_0_1.root", "darkPhotonRun2FSR_0_3.root"};
  // std::vector<std::string> hists = {
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput"
  // };
  
  // std::vector<TString> names = {"darkPhotonRun2FSR_0_1", "darkPhotonRun2FSR_0_3"};
  // std::vector<int> colors = {kBlack, kBlue};

  // TString xTitle = "BDT Output";
  // TString yTitle = "Number of Events";



  // darkPhotonBaselineRun2 vs. DY50-inf super imposed graphs
  // std::vector<std::string> files = {"darkPhotonBaselineRun2LeptonJetReconstruction.root", "DY50_inf.root"};
  // std::vector<std::string> hists = {
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput",
  //     "Low Mass and Different Sign__hists/Low Mass and Different Sign_LeptonJetMLOutput"
  // };
  
  // std::vector<TString> names = {"darkPhotonBaselineRun2", "DY50-inf"};
  // std::vector<int> colors = {kBlack, kBlue};

  // TString xTitle = "BDT Output";
  // TString yTitle = "Number of Events";




  int count = 0;
  TFile* openedFile;
  TH1* hist;
  std::vector<TH1*> histVector;
  std::string fileName;

  for (std::string file : files) {
    // Debug: Show the current file being processed
    std::cout << "Processing file: " << file << std::endl;
    
    fileName = filePath + file;
    openedFile = TFile::Open(fileName.c_str(), "read");
    
    if (!openedFile) {
      std::cerr << "Error: Cannot open file: " << fileName << std::endl;
      throw std::runtime_error("Cannot open file!");
    } else {
      std::cout << "Successfully opened file: " << fileName << std::endl;
    }

    std::string name = hists.at(count);
    uint pos = name.find("/");
    std::string folder = name.substr(0, pos);
    std::string histName = name.substr(pos + 1);

    // Debug: Show the folder and histogram name
    std::cout << "Looking for histogram: " << histName << " in folder: " << folder << std::endl;

    auto dir = openedFile->GetDirectory(folder.c_str());
    if (!dir) {
      std::cerr << "Error: Cannot find directory: " << folder << std::endl;
      throw std::runtime_error("Cannot find directory!");
    }

    hist = dynamic_cast<TH1*>(dir->Get(histName.c_str()));
    if (!hist) {
      std::cerr << "Error: Histogram " << histName << " not found in directory: " << folder << std::endl;
      throw std::runtime_error("Histogram " + histName + " not found!");
    }

    hist = dynamic_cast<TH1*>(hist->Clone());
    hist->SetName(("hist" + std::to_string(count)).c_str());
    std::cout << "Set name to " << hist->GetName() << std::endl;

    if (dynamic_cast<TH2*>(hist)) {
      TH2* hist2D = dynamic_cast<TH2*>(hist);
      TH1* newhist = hist2D->ProjectionX("_px", 0, -1, "E");
      histVector.push_back(newhist);

      // Debug: Inform about 2D histogram projection
      std::cout << "Converted 2D histogram to 1D (ProjectionX)" << std::endl;
    } else {
      histVector.push_back(hist);
      // Debug: Inform about 1D histogram
      std::cout << "Using 1D histogram: " << histName << std::endl;
    }

    count++;
  }

  // Debug: Check how many histograms were loaded
  std::cout << "Loaded " << histVector.size() << " histograms." << std::endl;

  // Debug: Before performing Add on histograms
  std::cout << "Adding histograms: " << std::endl;


  histVector[0]->Add(histVector[1]);

  // Use this if you have four histograms you want to superimpose
  // histVector[0]->Add(histVector[2]);
  // histVector[0]->Add(histVector[3]);

  // Re-assign histograms after addition
  histVector = {histVector[0], histVector[1]}; // Use this if you have two histograms
  //histVector = {histVector[0], histVector[1], histVector[2], histVector[3]}; // Use this if you have four histograms


  // Debug: Inform about the superimposition step
  std::cout << "Superimposing histograms..." << std::endl;

  TCanvas* canvas = plotFormatter->simpleSuperImposedHist(histVector, colors, names, xTitle, yTitle);

  // Debug: Inform that the plot was created
  std::cout << "Plotting completed!" << std::endl;
}
