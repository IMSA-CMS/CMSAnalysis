#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/MassFilter.hh"
#include "CIAnalysis/CIStudies/interface/CollinsSoperFilter.hh"
#include "CIAnalysis/CIStudies/interface/PileupFilter.hh"
#include "CIAnalysis/CIStudies/interface/MigrationModule.hh"
#include "CIAnalysis/CIStudies/interface/AcceptanceModule.hh"
#include "CIAnalysis/CIStudies/interface/ResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/PtResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/AFBModule.hh"
#include "CIAnalysis/CIStudies/interface/SimpleHistogramModule.hh"
#include "CIAnalysis/CIStudies/interface/UnmatchedParticleModule.hh"
#include "CIAnalysis/CIStudies/interface/PastingModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/InvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/PtHist.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
#include "CIAnalysis/CIStudies/interface/WeakAlignmentBiasModule.hh"

int main(int argc, char** argv)
{
  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser ("Analyze FWLite Histograms");
  parser.addOption("pileup", optutl::CommandLineParser::kString, "PileupLevel", "");
  parser.addOption("output", optutl::CommandLineParser::kString, "Particle", "");
  parser.addOption("input", optutl::CommandLineParser::kString, "Input", "");
  parser.parseArguments (argc, argv);

  std::string pileupLev = parser.stringValue("pileup");
  std::string inputFile = parser.stringValue("input");
  std::string outputFile = parser.stringValue("output");
  if (outputFile.empty())
    {
      outputFile = "genOutput.root";
    }

  std::cout << "This is the name of outputFile " << outputFile << std::endl;

  unsigned outputEvery = parser.integerValue("outputEvery");

  Analyzer analyzer;

  
  GenSimIdentificationModule genSimMod;
  RecoIdentificationModule recoMod;
  MatchingModule matchMod(genSimMod, recoMod);
  WeightingModule weightMod;
  LRWeightModule lrWeightMod;
  PtResolutionModule ptResMod(matchMod);
  MassFilter massFilter(genSimMod, 2000);
  CollinsSoperFilter csFilter(genSimMod, .975);
  PileupFilter pileupFilter(15, 35);
  MigrationModule migMod(matchMod);
  WeakAlignmentBiasModule weakAlignBiasMod(matchMod);
  AcceptanceModule accMod(genSimMod, matchMod);
  PtResolutionModule pTResMod(matchMod);
  MassResolutionModule massResMod(matchMod);
  AFBModule afbMod(genSimMod, recoMod);
  SimpleHistogramModule simpleMod(genSimMod, recoMod);
  UnmatchedParticleModule unmatchedMod(genSimMod, matchMod);
  PastingModule pasteMod(genSimMod, recoMod, weightMod, lrWeightMod);

  InvariantMassHist genSimInvMassHist(genSimMod, recoMod, true, "GenSim Invariant Mass Pasted", 54, 300, 5000);  // GenSim Invariant Mass Histogram
  InvariantMassHist recoInvMassHist(genSimMod, recoMod, false, "Reco Invariant Mass Pasted", 54, 300, 5000);     // Reco Invariant Mass Histogram
  PtHist genSimPtHist(genSimMod, recoMod, true, "GenSim Transverse Momentum Pasted", 54, 50, 1900);              // GenSim pT Histogram
  PtHist recoPtHist(genSimMod, recoMod, false, "Reco Transverse Momentum Pasted", 54, 50, 1900);                 // Reco pT Histogram

  // Add the four histograms created above to pasteMod
  pasteMod.addHistogram(&genSimInvMassHist);
  pasteMod.addHistogram(&recoInvMassHist);
  pasteMod.addHistogram(&genSimPtHist);
  pasteMod.addHistogram(&recoPtHist);
  
  analyzer.addProductionModule(&genSimMod);
  analyzer.addProductionModule(&recoMod);
  analyzer.addProductionModule(&matchMod);
  analyzer.addProductionModule(&weightMod);
  analyzer.addProductionModule(&lrWeightMod);
  //analyzer.addFilterModule(&massFilter);
  //analyzer.addFilterModule(&csFilter);
  //analyzer.addFilterModule(&pileupFilter);
  analyzer.addAnalysisModule(&migMod);
  analyzer.addAnalysisModule(&weakAlignBiasMod);
  //analyzer.addAnalysisModule(&accMod);
  analyzer.addAnalysisModule(&pTResMod);
  //analyzer.addAnalysisModule(&massResMod);
  //analyzer.addAnalysisModule(&afbMod);
  //analyzer.addAnalysisModule(&simpleMod);
  //analyzer.addAnalysisModule(&unmatchedMod);
  analyzer.addAnalysisModule(&pasteMod);

  if (inputFile.empty())
    {
      inputFile = "textfiles/pickFiles.txt";
    }

  analyzer.run(inputFile, outputFile, outputEvery);
  
  return 0;
}
