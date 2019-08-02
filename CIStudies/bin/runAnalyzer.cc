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
<<<<<<< HEAD
#include "CIAnalysis/CIStudies/interface/AcceptanceModule.hh"
#include "CIAnalysis/CIStudies/interface/ResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/PtResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/AFBModule.hh"
=======
#include "CIAnalysis/CIStudies/interface/PileupFilter.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
>>>>>>> 0c3825aa201e1579373efec2f43c967e450d5128

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
  PtResolutionModule pTResMod(matchMod);
  MassResolutionModule massResMod(matchMod);
  AFBModule afbMod(genSimMod, recoMod);
  

  analyzer.addProductionModule(&genSimMod);
  analyzer.addProductionModule(&recoMod);
  analyzer.addProductionModule(&matchMod);
<<<<<<< HEAD
  //analyzer.addAnalysisModule(&migMod);
  //analyzer.addAnalysisModule(&accMod);
  //analyzer.addAnalysisModule(&pTResMod);
  //analyzer.addAnalysisModule(&massResMod);
  analyzer.addAnalysisModule(&afbMod);
=======
  analyzer.addFilterModule(&pileupFilter);
  analyzer.addAnalysisModule(&migMod);
  analyzer.addAnalysisModule(&accMod);
>>>>>>> 0c3825aa201e1579373efec2f43c967e450d5128

  analyzer.run("textfiles/pickFiles.txt", outputFile, outputEvery);
  
  return 0;
}
