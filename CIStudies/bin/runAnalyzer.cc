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

int main(int argc, char** argv)
{
  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser ("Analyze FWLite Histograms");
  parser.addOption("pileup", optutl::CommandLineParser::kString, "PileupLevel", "");
  parser.addOption("output", optutl::CommandLineParser::kString, "Particle", "");
  parser.parseArguments (argc, argv);

  std::string pileupLev = parser.stringValue("pileup");
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
  MassFilter massFilter(genSimMod, 2000);
  CollinsSoperFilter csFilter(genSimMod, .975);
  PileupFilter pileupFilter(15, 35);
  MigrationModule migMod(matchMod);
  AcceptanceModule accMod(genSimMod, matchMod);
  PtResolutionModule pTResMod(matchMod);
  MassResolutionModule massResMod(matchMod);
  AFBModule afbMod(genSimMod, recoMod);
  SimpleHistogramModule simpleMod(genSimMod, recoMod);
  UnmatchedParticleModule unmatchedMod(genSimMod, matchMod);
  PastingModule pasteMod(genSimMod, recoMod, weightMod);
  
  analyzer.addProductionModule(&genSimMod);
  analyzer.addProductionModule(&recoMod);
  analyzer.addProductionModule(&matchMod);
  analyzer.addProductionModule(&weightMod);
  //analyzer.addFilterModule(&massFilter);
  //analyzer.addFilterModule(&csFilter);
  //analyzer.addFilterModule(&pileupFilter);
  analyzer.addAnalysisModule(&migMod);
  //analyzer.addAnalysisModule(&accMod);
  //analyzer.addAnalysisModule(&pTResMod);
  //analyzer.addAnalysisModule(&massResMod);
  //analyzer.addAnalysisModule(&afbMod);
  //analyzer.addAnalysisModule(&simpleMod);
  //analyzer.addAnalysisModule(&unmatchedMod);
  //analyzer.addAnalysisModule(&pasteMod);

  analyzer.run("textfiles/pickFiles.txt", outputFile, outputEvery);
  
  return 0;
}
