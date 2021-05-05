#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "CIAnalysis/CIStudies/bin/massResolutionAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/HPlusPlusMassAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/leptonJetReconstructionAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/massAcceptanceAnalysis.cc"


int main(int argc, char** argv)
{
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
      outputFile = "electronResolution.root";
    }

  std::cout << "This is the name of outputFile " << outputFile << std::endl;

  unsigned outputEvery = parser.integerValue("outputEvery");

  //Analyzer analyzer = hPlusPlusMassAnalysis();
  // Analyzer analyzer = hPlusPlusMassAnalysis();
  //Analyzer analyzer = massResolutionAnalysis();
  Analyzer analyzer = leptonJetReconstructionAnalysis();
  //Analyzer analyzer = displacedVertexAnalysis();
  Analyzer analyzer = massAcceptanceAnalysis();

  std::cout << "Notice: analyzer created" << std::endl;

  if (inputFile.empty())
    {
      inputFile = "textfiles/pickFiles.txt";
    }

  std::cout << "Notice: input file selected" << std::endl;
  //error in line below

  analyzer.run(inputFile, outputFile, outputEvery, numFiles);

  std::cout << "Notice: analyzer successfully ran" << std::endl;
  
  return 0;
}