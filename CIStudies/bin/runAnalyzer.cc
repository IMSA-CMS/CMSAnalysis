#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CIAnalysis/CIStudies/interface/Analyzer.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "CIAnalysis/CIStudies/bin/massResolutionAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/HPlusPlusMassAnalysis.cc"
#include "CIAnalysis/CIStudies/bin/leptonJetReconstructionAnalysis.cc"


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
      outputFile = "electronResolution.root";
    }

  std::cout << "This is the name of outputFile " << outputFile << std::endl;

  unsigned outputEvery = parser.integerValue("outputEvery");

  Analyzer analyzer = hPlusPlusMassAnalysis();
  //Analyzer analyzer = massResolutionAnalysis();
  //Analyzer analyzer = leptonJetReconstructionAnalysis();
  //Analyzer analyzer = displacedVertexAnalysis();

  std::cout << "Notice: analyzer created" << std::endl;

  if (inputFile.empty())
    {
      inputFile = "textfiles/pickFiles.txt";
    }

  std::cout << "Notice: input file selected" << std::endl;
  //error in line below

  analyzer.run(inputFile, outputFile, outputEvery);

  std::cout << "Notice: analyzer successfully ran" << std::endl;
  
  return 0;
}