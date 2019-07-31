#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "CIAnalysis/CIStudies/interface/AcceptanceModule.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/MigrationModule.hh"
#include "CIAnalysis/CIStudies/interface/PileupFilter.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

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
  PileupFilter pileupFilter(15, 35);
  MigrationModule migMod(matchMod);
  AcceptanceModule accMod(genSimMod, matchMod);

  analyzer.addProductionModule(&genSimMod);
  analyzer.addProductionModule(&recoMod);
  analyzer.addProductionModule(&matchMod);
  analyzer.addFilterModule(&pileupFilter);
  analyzer.addAnalysisModule(&migMod);
  analyzer.addAnalysisModule(&accMod);

  analyzer.run("textfiles/pickFiles.txt", outputFile, outputEvery);
  
  return 0;
}
