#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
#include "CIAnalysis/CIStudies/bin/massResolutionAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/HPlusPlusMassAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/leptonJetReconstructionAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/displacedVertexAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/massAcceptanceAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/triggerAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/invariantMassAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/HiggsBackgroundAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/LeptonJetBackgroundAnalysis.cc"


int main(int argc, char **argv) {
  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser ("Analyze FWLite Histograms");

  parser.addOption("output", optutl::CommandLineParser::kString, "Particle", "");

  parser.addOption("input", optutl::CommandLineParser::kString, "Input", "");
  parser.addOption("numFiles", optutl::CommandLineParser::kInteger, "Number of Files", -1);
  parser.parseArguments (argc, argv);

  std::string inputFile = parser.stringValue("input");

  std::string outputFile = parser.stringValue("output");
  int numFiles = parser.integerValue("numFiles");

  if (outputFile.empty())
    {
      outputFile = "default.root";
    }


  std::cout << "Reading input file " << inputFile << std::endl;

  unsigned outputEvery = parser.integerValue("outputEvery");

  // USING ANALYER FOR H++ THAT WON'T CHANGE: CHANGE BACK
  // TO hPlusPlusMassAnalysis() when ready -- [09/17]
  //Analyzer analyzer = leptonJetBackgroundAnalysis();
  //Analyzer analyzer = massResolutionAnalysis();
  //Analyzer analyzer = leptonJetReconstructionAnalysis();
  //Analyzer analyzer = displacedVertexAnalysis();
  //Analyzer analyzer = massAcceptanceAnalysis();
  //Analyzer analyzer = massAcceptanceAnalysis();
  //Analyzer analyzer = triggerAnalysis();
  Analyzer analyzer = invariantMassAnalysis();
  //Analyzer analyzer = higgsBackgroundAnalysis();
  

  if (inputFile.empty())
  {
    inputFile = "textfiles/pickFiles.txt";
  }

  analyzer.run(inputFile, outputFile, outputEvery, numFiles);

  std::cout << "Processing complete!" << std::endl;
  std::cout << "Output written to " << outputFile << std::endl;

  return 0;
}
