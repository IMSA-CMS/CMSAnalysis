#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"
#include "TMVA/IMethod.h"
#include "CMSAnalysis/Modules/interface/LeptonJetMLStripModule.hh"

//caete mlstrpi, and use addto dataloader

// To predict if a lepton jet is real, I am using a combination of CUDA-accelerated
// and Multi-core accelerated DNN's. I am using all of the data reduction methods.
// The work left to do is to replace the training dataset used with the generated
// dataset and change the names to build without errors. (10/24/2021)

/*
s/sqrt(B)
B = cross*luminosity

dy50: 5711,
qcd2000: 6.458
*/

//bg dy50run2
//sg darkphotonbaselinerun2

// Structure to hold the parsed training parameters.
struct TrainingParams {
    std::string sgMethod;
    std::string bgMethod;
    int useDP;
    int useNano;
    int useDY;
    int useQCD;
};

// Helper function that parses trainingArgs into a TrainingParams structure.
bool parseTrainingArgs(const std::string &trainingArgs, TrainingParams &params) {
    std::unordered_map<std::string, std::string> args;
    std::istringstream iss(trainingArgs);
    std::string token;

    // Tokenize the input string using whitespace.
    while (iss >> token) {
        size_t pos = token.find('=');
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::string value = token.substr(pos + 1);
            args[key] = value;
        }
    }

    // Check that all required keys are present.
    if (args.find("sgMethod") == args.end() ||
        args.find("bgMethod") == args.end() ||
        args.find("useDP") == args.end() ||
        args.find("useNano") == args.end() ||
        args.find("useDY") == args.end() ||
        args.find("useQCD") == args.end())
    {
        std::cerr << "Missing one or more required training arguments." << std::endl;
        return false;
    }

    // Assign and convert values to the TrainingParams structure.
    params.sgMethod = args["sgMethod"];
    params.bgMethod = args["bgMethod"];
    try {
        params.useDP   = std::stoi(args["useDP"]);
        params.useNano = std::stoi(args["useNano"]);
        params.useDY   = std::stoi(args["useDY"]);
        params.useQCD  = std::stoi(args["useQCD"]);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error converting one of the integer arguments: " << e.what() << std::endl;
        return false;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "One of the integer arguments is out of range: " << e.what() << std::endl;
        return false;
    }

    return true;
}

// Example trainingArgs usage: "sgMethod=methodA bgMethod=methodB useDP=1 useNano=0 useDY=1 useQCD=0"

bool returnState(TString &myMethodList, std::string trainPath, std::string outputPath, std::string outFile, std::string trainingArgs) {
  
  TrainingParams params;

  // Call the parsing function.
  if (!parseTrainingArgs(trainingArgs, params)) {
      return false;
  }

  std::cout << "Parsed parameters:" << std::endl;
  std::cout << "  sgMethod: " << params.sgMethod << std::endl;
  std::cout << "  bgMethod: " << params.bgMethod << std::endl;
  std::cout << "  useDP: "    << params.useDP    << std::endl;
  std::cout << "  useNano: "  << params.useNano  << std::endl;
  std::cout << "  useDY: "    << params.useDY    << std::endl;
  std::cout << "  useQCD: "   << params.useQCD   << std::endl;

  TMVA::Tools::Instance();

  std::map<std::string, int> Use;

  // Cut optimisation
  Use["Cuts"] = 0;
  Use["CutsD"] = 0;
  Use["CutsPCA"] = 0;
  Use["CutsGA"] = 0;
  Use["CutsSA"] = 0;

  // Neural Networks (all are feed-forward Multilayer Perceptrons)
  Use["MLP"] = 0;       // Recommended ANN
  Use["MLPBFGS"] = 0;   // Recommended ANN with optional training method
  Use["MLPBNN"] = 0;    // Recommended ANN with BFGS training method and bayesian regulator
  Use["CFMlpANN"] = 0;  // Depreciated ANN from ALEPH
  Use["TMlpANN"] = 0;   // ROOT's own ANN

  // Boosted Decision Trees
  Use["BDT"] = 1;   // Boosted Decision Trees (Adaptive Boost, recommended)
  Use["BDTG"] = 0;  // uses Gradient Boost
  Use["BDTB"] = 0;  // uses Bagging
  Use["BDTD"] = 0;  // decorrelation + Adaptive Boost
  Use["BDTF"] = 0;  // allow usage of fisher discriminant for node splitting

  // Validation tools
  Use["CV"] = 0; // Performs cross-validation and prints summary statistics (NOTE: Not saved in output root file, so copy elsewhere if you actually want these)

#ifdef R__HAS_TMVAGPU
  Use["DNN_GPU"] = 0;  // CUDA-accelerated DNN training.
#else
  Use["DNN_GPU"] = 0;
#endif

#ifdef R__HAS_TMVACPU
  Use["DNN_CPU"] = 0;  // Multi-core accelerated DNN.
#else
  Use["DNN_CPU"] = 0;
#endif

  std::cout << std::endl;
  std::cout << "==> Start TMVAClassification" << std::endl;

  // Select methods (don't look at this code - not of interest)
  if (myMethodList != "") {
    for (std::map<std::string, int>::iterator it = Use.begin(); it != Use.end(); it++)
      it->second = 0;

    std::vector<TString> mlist = TMVA::gTools().SplitString(myMethodList, ',');
    for (UInt_t i = 0; i < mlist.size(); i++) {
      std::string regMethod(mlist[i]);

      if (Use.find(regMethod) == Use.end()) {
        std::cout << "Method \"" << regMethod
                  << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
        for (std::map<std::string, int>::iterator it = Use.begin(); it != Use.end(); it++)
          std::cout << it->first << " ";
        std::cout << std::endl;
        return 1;
      }
      Use[regMethod] = 1;
    }
  }

  // input signal file here
  //std::string sgFile = "BDTFiles/strippedSG_LMSS_numFiles1.root";
  std::vector<std::string> nanoFiles = {
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_1_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_125_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_300_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_1_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_3_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_4_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_6_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_9_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_1_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_1_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_2_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_4_0_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_Higgs4DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    "DPNanoFiles/DarkPhoton_Decay_HiggsDPZ_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //"DPNanoFiles/DarkPhoton_Decay_SUSY_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
    //"DPNanoFiles/DarkPhoton_Decay_ZPrime_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root",
  };

  // std::vector<std::string> dpBaselineFiles = {
  //   "BDTFiles/strippedSG_numFiles1.root"
  // };

  std::vector<std::string> dpBaselineFiles = {
    trainPath + "darkPhotonBaselineRun2.root"
  };

  std::map<std::string, double> nanoCrossSections = {
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_1_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_125_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_300_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_1_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_3_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_4_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_6_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_0_9_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_1_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_1_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_2_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs2DP_DpMass_4_0_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_Higgs4DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_HiggsDPZ_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_SUSY_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1},
    {"DPNanoFiles/DarkPhoton_Decay_ZPrime_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.root", 1}
  };

  // std::map<std::string, double> dpCrossSections = {
  //   {"BDTFiles/strippedSG_numFiles1.root", 1},
  // };

  std::map<std::string, double> dpCrossSections = {
    {trainPath + "darkPhotonBaselineRun2.root", 1},
  };

  std::vector<std::string> sgFiles = {};
  std::map<std::string, double> sgCrossSections = {};

  if (params.useDP == 1)
  {
    sgFiles.insert(sgFiles.end(), dpBaselineFiles.begin(), dpBaselineFiles.end());
    sgCrossSections.insert(dpCrossSections.begin(), dpCrossSections.end());
  }

  if (params.useNano == 1)
  {
    sgFiles.insert(sgFiles.end(), nanoFiles.begin(), nanoFiles.end());
    sgCrossSections.insert(nanoCrossSections.begin(), nanoCrossSections.end());
  }

  // input background files here
  //std::string bgFiles[] = {"BDTFiles/strippedBG_LMSS_numFiles1.root"};
  // std::vector<std::string> qcdFiles = {
  //   "BDTFiles/strippedBG_QCD100-200_numFiles1.root",
  //   "BDTFiles/strippedBG_QCD200-300_numFiles1.root",
  //   "BDTFiles/strippedBG_QCD300-500_numFiles1.root",
  //   "BDTFiles/strippedBG_QCD500-700_numFiles1.root",
  //   "BDTFiles/strippedBG_QCD700-1000_numFiles1.root",
  //   "BDTFiles/strippedBG_QCD1000-1500_numFiles1.root",
  //   "BDTFiles/strippedBG_QCD1500-2000_numFiles1.root",
  //   "BDTFiles/strippedBG_QCD2000-inf_numFiles1.root"
  // };

  std::vector<std::string> qcdFiles = {
    trainPath + "QCD1000-1500.root",
    trainPath + "QCD100-200.root",
    trainPath + "QCD1500-2000.root",
    trainPath + "QCD2000-inf.root",
    trainPath + "QCD200-300.root",
    trainPath + "QCD300-500.root",
    trainPath + "QCD500-700.root",
    trainPath + "QCD700-1000.root",
  };

  std::vector<std::string> dyFiles = {
    trainPath + "DY10-50.root",
    trainPath + "DY50-inf.root",
  };

  // std::map<std::string, double> qcdCrossSections = {
  //   {"BDTFiles/strippedBG_QCD100-200_numFiles1.root", 1122000},
  //   {"BDTFiles/strippedBG_QCD200-300_numFiles1.root", 79760},
  //   {"BDTFiles/strippedBG_QCD300-500_numFiles1.root", 16600},
  //   {"BDTFiles/strippedBG_QCD500-700_numFiles1.root", 1503},
  //   {"BDTFiles/strippedBG_QCD700-1000_numFiles1.root", 297.4},
  //   {"BDTFiles/strippedBG_QCD1000-1500_numFiles1.root", 48.08},
  //   {"BDTFiles/strippedBG_QCD1500-2000_numFiles1.root", 3.951},
  //   {"BDTFiles/strippedBG_QCD2000-inf_numFiles1.root", 0.6957}
  // };

  std::map<std::string, double> qcdCrossSections = {
    {trainPath + "QCD100-200.root", 1122000},
    {trainPath + "QCD200-300.root", 79760},
    {trainPath + "QCD300-500.root", 16600},
    {trainPath + "QCD500-700.root", 1503},
    {trainPath + "QCD700-1000.root", 297.4},
    {trainPath + "QCD1000-1500.root", 48.08},
    {trainPath + "QCD1500-2000.root", 3.951},
    {trainPath + "QCD2000-inf.root", 0.6957}
  };

  std::map<std::string, double> dyCrossSections = {
    {trainPath + "DY10-50.root", 20460},
    {trainPath + "DY50-inf.root", 5735}
  };

  std::vector<std::string> bgFiles = {};
  std::map<std::string, double> bgCrossSections = {};

  if (params.useQCD == 1)
  {
    bgFiles.insert(bgFiles.end(), qcdFiles.begin(), qcdFiles.end());
    bgCrossSections.insert(qcdCrossSections.begin(), qcdCrossSections.end());
  }

  if (params.useDY == 1)
  {
    bgFiles.insert(bgFiles.end(), dyFiles.begin(), dyFiles.end());
    bgCrossSections.insert(dyCrossSections.begin(), dyCrossSections.end());
  }

  // (it is also possible to use ASCII format as input -> see TMVA Users Guide)
  TFile *input(0);
  TString fname = "./tmva_class_example.root";
  if (!gSystem->AccessPathName(fname)) {
    input = TFile::Open(fname);  // check if file in local directory exists
  } else {
    TFile::SetCacheFileDir(".");
    input = TFile::Open(sgFiles[0].c_str(), "CACHEREAD");  //
  }
  if (!input) {
    std::cout << "ERROR: could not open data file" << std::endl;
    exit(1);
  }
  std::cout << "--- TMVAClassification       : Using input file: " << input->GetName() << std::endl;

  // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
  TString outfileName(outFile + ".root");
  TFile *outputFile = TFile::Open(outfileName, "RECREATE");

  TString opt = "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification";

  TMVA::Factory *factory =
      new TMVA::Factory("TMVAClassification",
                        outputFile,
                        opt);

  TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

  // change var1 to index and var2 to pt
  LeptonJetMLStripModule leptonMod;
  leptonMod.initialize();
  leptonMod.addVariablesToDataLoader(dataloader);
  // Difference in transverse momentum between leading and runner up highest transverse momentum particles.
  
  double totalSgCrossSection = 0;
  for (std::string file : sgFiles) {
    totalSgCrossSection += sgCrossSections[file];
  }

  double totalBgCrossSection = 0;
  for (std::string file : bgFiles) {
    totalBgCrossSection += bgCrossSections[file];
  }

  
  ////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////// START BG EVENT SELECTION ///////////////////////////////////
  ///////////////////////////// WARNING: ONLY UNCOMMENT ONE SECTION //////////////////////////////
  ///////////////////////// CODE WILL COMPILE AND RUN (SO NO ERROR MESSAGE) //////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////

  if (strcmp(params.sgMethod.c_str(), "PropWeight") == 0)
  {
      ////////////////////// Proportional Weight //////////////////////
      for (std::string file : bgFiles) {
        TChain *backgroundTree = new TChain("Signal");  // Signal of background files
        backgroundTree->Add(file.c_str());
        dataloader->AddBackgroundTree(backgroundTree, bgCrossSections[file] / totalBgCrossSection);
      }
  }
  else if (strcmp(params.sgMethod.c_str(), "Uniform") == 0)
  {
      ////////////////////// Uniform Unweighted //////////////////////
      TChain *backgroundTree = new TChain("Signal");  // Signal of background files
      for (std::string file : bgFiles) {
        backgroundTree->Add(file.c_str());
      }
      dataloader->AddBackgroundTree(backgroundTree, 1.0);
  }

  ////////////////////// Proportional Split //////////////////////

  // std::map<std::string, int> numEventsToLoad;

  // for (const auto &file : crossSections) {
  //   TFile *bgFile = TFile::Open(file.first.c_str());
  //   TTree *tree = (TTree *)bgFile->Get("Signal");
  //   int totalEntries = tree->GetEntries();
  //   numEventsToLoad[file.first] = (int)((file.second / totalCrossSection) * totalEntries);
  //   bgFile->Close();
  // }

  // // Now, we use TEntryList to select the events proportionally
  // TChain *backgroundTree = new TChain("Signal");

  // for (const auto &file : crossSections) {
  //     TFile *bgFile = TFile::Open(file.first.c_str());
  //     TTree *tree = (TTree *)bgFile->Get("Signal");

  //     int entriesToLoad = numEventsToLoad[file.first];
  //     TEntryList *entryList = new TEntryList(tree);
  //     for (int i = 0; i < entriesToLoad; ++i) {
  //         entryList->Enter(i);
  //     }

  //     tree->SetEntryList(entryList);
  //     backgroundTree->Add(file.first.c_str());
  //     delete entryList;
  //     bgFile->Close();
  // }

  // dataloader->AddBackgroundTree(backgroundTree, 1.0);

  ///////////////////////// Inverse Split ////////////////////////

  // std::map<std::string, int> numEventsToLoad;

  // for (const auto &file : crossSections) {
  //   TFile *bgFile = TFile::Open(file.first.c_str());
  //   TTree *tree = (TTree *)bgFile->Get("Signal");
  //   int totalEntries = tree->GetEntries();
  //   numEventsToLoad[file.first] = (int)((1.0 - (file.second / totalCrossSection)) * totalEntries);
  //   bgFile->Close();
  // }

  // // Now, we use TEntryList to select the events proportionally
  // TChain *backgroundTree = new TChain("Signal");

  // for (const auto &file : crossSections) {
  //     TFile *bgFile = TFile::Open(file.first.c_str());
  //     TTree *tree = (TTree *)bgFile->Get("Signal");

  //     int entriesToLoad = numEventsToLoad[file.first];
  //     TEntryList *entryList = new TEntryList(tree);
  //     for (int i = 0; i < entriesToLoad; ++i) {
  //         entryList->Enter(i);
  //     }

  //     tree->SetEntryList(entryList);
  //     backgroundTree->Add(file.first.c_str());
  //     delete entryList;
  //     bgFile->Close();
  // }

  // dataloader->AddBackgroundTree(backgroundTree, 1.0);

  ////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////// END BG EVENT SELECTION ////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////// START SG EVENT SELECTION ///////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////

  if (strcmp(params.sgMethod.c_str(), "PropWeight") == 0) 
  {
    ////////////////////// Proportional Weight //////////////////////

    for (std::string file : sgFiles) {
      TChain *signalTree = new TChain("Signal");  // Signal of background files
      signalTree->Add(file.c_str());
      dataloader->AddSignalTree(signalTree, sgCrossSections[file] / totalSgCrossSection);
    }

  } else if (strcmp(params.sgMethod.c_str(), "Uniform") == 0) {

    ////////////////////// Uniform Unweighted //////////////////////

    TChain *signalTree = new TChain("Signal");  // Signal of background files
    for (std::string file : sgFiles) {
      signalTree->Add(file.c_str());
    }
    dataloader->AddSignalTree(signalTree, 1.0);

  }

  ////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////// END SG EVENT SELECTION ////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////

  // I am using all of the data reduction methods
  if (Use["Cuts"])
    factory->BookMethod(
        dataloader, TMVA::Types::kCuts, "Cuts", "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart");

  if (Use["CutsD"])
    factory->BookMethod(dataloader,
                        TMVA::Types::kCuts,
                        "CutsD",
                        "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate");

  if (Use["CutsPCA"])
    factory->BookMethod(dataloader,
                        TMVA::Types::kCuts,
                        "CutsPCA",
                        "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA");

  if (Use["CutsGA"])
    factory->BookMethod(dataloader,
                        TMVA::Types::kCuts,
                        "CutsGA",
                        "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles="
                        "3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95");

  if (Use["CutsSA"])
    factory->BookMethod(dataloader,
                        TMVA::Types::kCuts,
                        "CutsSA",
                        "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-"
                        "6:Eps=1e-10:UseDefaultScale");

  if (Use["MLP"])
    factory->BookMethod(dataloader,
                        TMVA::Types::kMLP,
                        "MLP",
                        "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator");

  if (Use["BDT"])
    factory->BookMethod(dataloader,
                        TMVA::Types::kBDT,
                        "BDT",
                        "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:"
                        "UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20");

  // Using both CUDA-Accelerated and Multi-core Accelerated Deep Neural Networks
  if (Use["DNN_CPU"] or Use["DNN_GPU"]) {
    // General layout

    TString layoutString("Layout=TANH|128,TANH|128,TANH|128,LINEAR");

    // Define Training strategy. One could define multiple strategy string separated by the "|" delimiter

    TString trainingStrategyString =
        ("TrainingStrategy=LearningRate=1e-2,Momentum=0.9,"
         "ConvergenceSteps=20,BatchSize=100,TestRepetitions=1,"
         "WeightDecay=1e-4,Regularization=None,"
         "DropConfig=0.0+0.5+0.5+0.5");

    // General Options
    TString dnnOptions(
        "!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
        "WeightInitialization=XAVIERUNIFORM");
    dnnOptions.Append(":");
    dnnOptions.Append(layoutString);
    dnnOptions.Append(":");
    dnnOptions.Append(trainingStrategyString);

    // Cuda implementation
    if (Use["DNN_GPU"]) {
      TString gpuOptions = dnnOptions + ":Architecture=GPU";
      factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_GPU", gpuOptions);
    }
    // Multi-core CPU implementation
    if (Use["DNN_CPU"]) {
      TString cpuOptions = dnnOptions + ":Architecture=CPU";
      factory->BookMethod(dataloader, TMVA::Types::kDL, "DNN_CPU", cpuOptions);
    }
  }

  // Train MVAs using the set of training events
  factory->TrainAllMethods();

  // Evaluate all MVAs using the set of test events
  factory->TestAllMethods();

  // Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();

  // Save the output
  outputFile->Close();

  std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;

  TFile *f = new TFile((outFile + "Rerun.root").c_str(), "recreate");
  factory->Write();
  f->Close();

  if (Use["CV"])
  {
    std::cout << "Beginning cross-validation:" << std::endl;

    // WARNING: I arbitrarily set input to sgFiles[0] at the top so beware because I'm not actually what this does here and I'm too lazy to find out
    TMVA::CrossValidation cv {"tescv", dataloader, input, opt}; 
    cv.BookMethod(TMVA::Types::kBDT, "BDT", "");
    cv.Evaluate();

    size_t iMethod = 0;
    for (auto && result : cv.GetResults()) {
      std::cout << "Summary for method "
                << cv.GetMethods()[iMethod++].GetValue<TString>("MethodName")
                << std::endl;
      for (UInt_t iFold = 0; iFold<cv.GetNumFolds(); ++iFold) {
        std::cout << "\tFold " << iFold << ": "
                  << "ROC int: " << result.GetROCValues()[iFold]
                  << ", "
                  << "BkgEff@SigEff=0.3: " << result.GetEff30Values()[iFold]
                  << std::endl;
      }
    }

    std::cout << "Completed cross-validaton!" << std::endl;
  }

  delete factory;
  delete dataloader;

  // Launch the GUI for the root macros
  if (!gROOT->IsBatch())
    TMVA::TMVAGui(outfileName);
  return false;
}

// Usage:
// @param sgMethod : method of signal event population : "PropWeight", "Uniform"
// @param bgMethod : method of background event population : "PropWeight", "Uniform"
// @param useDP : whether or not to train on DP baseline file : 0, 1
// @param useNano : whether or not to train on Nano DP signal files : 0, 1
// @param useDY : whether or not to train on DY background files : 0, 1
// @param useQCD : whether or not to train on QCD background files : 0, 1

void crab_BDT_MLTrain(std::string trainPath, std::string outputPath, std::string outFile, std::string trainArgs)
{
  // Select methods (don't look at this code - not of interest)
  TString methodList;
  returnState(methodList, trainPath, outputPath, outFile, trainArgs);
}