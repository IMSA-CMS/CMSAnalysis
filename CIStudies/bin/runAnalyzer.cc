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
#include "CIAnalysis/CIStudies/interface/BarrelStateFilter.hh"
#include "CIAnalysis/CIStudies/interface/MassBinFilter.hh"
#include "CIAnalysis/CIStudies/interface/AcceptanceModule.hh"
#include "CIAnalysis/CIStudies/interface/ResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/PtResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/AFBModule.hh"
#include "CIAnalysis/CIStudies/interface/UnmatchedParticleModule.hh"
#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/InvariantMassHist.hh"
#include "CIAnalysis/CIStudies/interface/PtHist.hh"
#include "CIAnalysis/CIStudies/interface/MassResolutionHist.hh"
#include "CIAnalysis/CIStudies/interface/LRWeightModule.hh"
// #include "CIAnalysis/CIStudies/interface/WeakAlignmentBiasModule.hh"
#include "CIAnalysis/CIStudies/interface/ScaledMassHist.hh"

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
      outputFile = "weakAlignBiasOutput.root";
    }

  std::cout << "This is the name of outputFile " << outputFile << std::endl;

  unsigned outputEvery = parser.integerValue("outputEvery");

  Analyzer analyzer;

  GenSimIdentificationModule genSimMod;
  RecoIdentificationModule recoMod;
  MatchingModule matchMod(genSimMod, recoMod);
  WeightingModule weightMod;
  LRWeightModule lrWeightMod;
  MassFilter massFilter(genSimMod, 2000);
  CollinsSoperFilter csFilter(genSimMod, .975);
  PileupFilter pileupFilter(15, 35);
  BarrelStateFilter barrelStateFilter(matchMod);
  MassBinFilter massBinFilter(matchMod, 300, 3100, 28);
  AcceptanceModule accMod(genSimMod, recoMod, weightMod, lrWeightMod, matchMod);
  PtResolutionModule pTResMod(genSimMod, recoMod, weightMod, lrWeightMod, matchMod);
  MassResolutionModule massResMod(genSimMod, recoMod, weightMod, lrWeightMod, matchMod);
  AFBModule afbMod(genSimMod, recoMod, weightMod, lrWeightMod);
  UnmatchedParticleModule unmatchedMod(genSimMod, recoMod, weightMod, lrWeightMod, matchMod);
  HistogramOutputModule histMod(genSimMod, recoMod, weightMod, lrWeightMod);

  // GenSim Invariant Mass Histogram
  // InvariantMassHist genSimInvMassHist(genSimMod, recoMod, true, "GenSim Invariant Mass Pasted", 640, 100, 3300);
  // Reco Invariant Mass Histogram
  // InvariantMassHist recoInvMassHist(genSimMod, recoMod, false, "Reco Invariant Mass Pasted", 640, 100, 3300);
  // GenSim pT Histogram
  // PtHist genSimPtHist(genSimMod, recoMod, true, "GenSim Transverse Momentum Pasted", 54, 50, 1900);
  // Reco pT Histogram
  // PtHist recoPtHist(genSimMod, recoMod, false, "Reco Transverse Momentum Pasted", 54, 50, 1900);
  // MassResolutionHist massResHist(genSimMod, recoMod, "Mass Resolution Pasted", 100, 500, 3100);                // Mass resolution Histogram

  // GenSim Invariant Mass Histogram
  InvariantMassHist genSimInvMassHist(genSimMod, recoMod, true, "GenSim Invariant Mass Pasted", 29, 300, 3200);
  // Reco Invariant Mass Histogram
  InvariantMassHist recoInvMassHist(genSimMod, recoMod, false, "Reco Invariant Mass Pasted", 29, 300, 3200);

  // GenSim Scaled Up Invariant Mass Histogram
  ScaledMassHist genSimScaledUpHist(matchMod, "GenSim Weak Alignment Bias Scaled Up Hist", 29, 300, 3200, true, false, true);
  // Reco Scaled Up Invariant Mass Histogram
  ScaledMassHist recoScaledUpHist(matchMod, "Reco Weak Alignment Bias Scaled Up Hist", 29, 300, 3200, true, false, false);

  // GenSim Scaled Down Invariant Mass Histogram
  ScaledMassHist genSimScaledDownHist(matchMod, "GenSim Weak Alignment Bias Scaled Down Hist", 29, 300, 3200, false, false, true);
  // Reco Scaled Down Invariant Mass Histogram
  ScaledMassHist recoScaledDownHist(matchMod, "Reco Weak Alignment Bias Scaled Down Hist", 29, 300, 3200, false, false, false);

  // GenSim Mu Scaled Up AntiMu Scaled Down Invariant Mass Histogram
  ScaledMassHist genSimScaledUpDownHist(matchMod, "GenSim Weak Alignment Bias Mu Scaled Up AntiMu Scaled Down Hist", 29, 300, 3200, false, true, true);
  // Reco Mu Scaled Up AntiMu Scaled Down Invariant Mass Histogram
  ScaledMassHist recoScaledUpDownHist(matchMod, "Reco Weak Alignment Bias Mu Scaled Up AntiMu Scaled Down Hist", 29, 300, 3200, false, true, false);


  // Add the filter modules to the five histograms created above
  // genSimInvMassHist.addFilter(&massBinFilter);
  // recoInvMassHist.addFilter(&massBinFilter);
  // genSimPtHist.addFilter(&massBinFilter);
  // recoPtHist.addFilter(&massBinFilter);

  // Add the five histograms created above to histMod

  histMod.addHistogram(&genSimInvMassHist);
  histMod.addHistogram(&recoInvMassHist);
  //histMod.addHistogram(&genSimPtHist);
  //histMod.addHistogram(&recoPtHist);
  //histMod.addHistogram(&massResHist);
  histMod.addHistogram(&genSimScaledUpHist);
  histMod.addHistogram(&recoScaledUpHist);
  histMod.addHistogram(&genSimScaledDownHist);
  histMod.addHistogram(&recoScaledDownHist);
  histMod.addHistogram(&genSimScaledUpDownHist);
  histMod.addHistogram(&recoScaledUpDownHist);

  analyzer.addProductionModule(&genSimMod);
  analyzer.addProductionModule(&recoMod);
  analyzer.addProductionModule(&matchMod);
  analyzer.addProductionModule(&weightMod);
  analyzer.addProductionModule(&lrWeightMod);
  //analyzer.addFilterModule(&massFilter);
  //analyzer.addFilterModule(&csFilter);
  //analyzer.addFilterModule(&pileupFilter);
  //analyzer.addFilterModule(&barrelStateFilter);
  //analyzer.addFilterModule(&massBinFilter);
  //analyzer.addAnalysisModule(&accMod);
  analyzer.addAnalysisModule(&pTResMod);
  analyzer.addAnalysisModule(&massResMod);
  //analyzer.addAnalysisModule(&afbMod);
  //analyzer.addAnalysisModule(&unmatchedMod);
  analyzer.addAnalysisModule(&histMod);

  if (inputFile.empty())
    {
      inputFile = "textfiles/pickFiles.txt";
    }

  analyzer.run(inputFile, outputFile, outputEvery);
  
  return 0;
}
