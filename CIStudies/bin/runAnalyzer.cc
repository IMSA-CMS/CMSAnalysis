#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "CIAnalysis/CIStudies/bin/HPlusPlusMassAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/leptonJetReconstructionAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/massAcceptanceAnalysis.cc"

int main(int argc, char **argv) {
  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser ("Analyze FWLite Histograms");

  parser.addOption("output-path", optutl::CommandLineParser::kString, "Particle", "");
  parser.addOption("output", optutl::CommandLineParser::kString)
  
  parser.addOption("input", optutl::CommandLineParser::kString, "Input", "");
  parser.addOption("numFiles", optutl::CommandLineParser::kInteger, "Number of Files", -1);
  parser.parseArguments (argc, argv);
  
  std::string inputFile = parser.stringValue("input");

  if(!parser.stringValue("output").empty())
  {
    if(!parser.stringValue("output-path").empty())
    {
      std::cout << "Warning: Two output options selected. Option output-path will not be used.";
    }
    
    outputFile = "./output/" + parser.stringValue("output");
  }
  else if(!parser.stringValue("output-path").empty())
  {
    outputFile = parser.stringValue("output-path");
  }
  else
  {
    outputFile = "./output/electronResolution.root";
  }
  
  int numFiles = parser.integerValue("numFiles");


  std::cout << "This is the name of outputFile " << outputFile << std::endl;

  unsigned outputEvery = parser.integerValue("outputEvery");


  // Analyzer analyzer = hPlusPlusMassAnalysis();
  // Analyzer analyzer = massResolutionAnalysis();
  // Analyzer analyzer = leptonJetReconstructionAnalysis();
  // Analyzer analyzer = displacedVertexAnalysis();
  Analyzer analyzer = massAcceptanceAnalysis();


  std::cout << "Notice: analyzer created" << std::endl;

  if (inputFile.empty()) {
    inputFile = "textfiles/pickFiles.txt";
  }

  std::cout << "Notice: input file selected" << std::endl;
  // error in line below

  analyzer.run(inputFile, outputFile, outputEvery, numFiles);

  std::cout << "Notice: analyzer successfully ran" << std::endl;

  return 0;
}
