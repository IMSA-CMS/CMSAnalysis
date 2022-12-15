#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/IDType.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/AnalyzerOptions.hh"
//#include "CMSAnalysis/DataCollection/bin/massResolutionAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/HPlusPlusMassAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/leptonJetReconstructionAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/displacedVertexAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/massAcceptanceAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/triggerAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/invariantMassAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/HiggsBackgroundAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/LeptonJetBackgroundAnalysis.cc"
//#include "CMSAnalysis/DataCollection/bin/FilestripAnalysis.cc"


int main(int argc, char **argv)
{
  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser("Analyze FWLite Histograms");

  parser.addOption("output", optutl::CommandLineParser::kString, "Particle", "");

  parser.addOption("input", optutl::CommandLineParser::kString, "Input", "");
  parser.addOption("numFiles", optutl::CommandLineParser::kInteger, "Number of Files", -1);

  parser.addOption("analysis", optutl::CommandLineParser::kString, "Type of Analysis", "");
  parser.parseArguments(argc, argv);

  std::string inputFile = parser.stringValue("input");

  std::string outputFile = parser.stringValue("output");
  int numFiles = parser.integerValue("numFiles");
  std::string analysisType = parser.stringValue("analysis");

  AnalyzerOptions options;
  if (inputFile.empty())
  {
    inputFile = options.pickfileInterface();
    // inputFile = "textfiles/pickFiles.txt";
  }

  if (outputFile.empty())
  {
    outputFile = "default.root";
  }

  unsigned outputEvery = parser.integerValue("outputEvery");

  //   Selection of data collection plan has moved to command line argument "analysis"
  //   The key for each Plan can now be found in AnalyzerOptions.cc

  //   e.g. runAnalyzer input=input.txt output=output.root analysis=DisplacedVertex

  std::string analysis = options.checkSelectedAnalysis(analysisType);
  std::cout << "Reading input file " << inputFile << std::endl;
  // analyzer.run(inputFile, outputFile, outputEvery, numFiles);

  // analysisPlans[analysisType]->runAnalyzer(inputFile, outputFile, outputEvery, numFiles);

  DataCollectionPlan *plan = options.getAnalysisPlans().at(analysis);
  plan->initialize();
  plan->runEventLoader(inputFile, outputFile, outputEvery, numFiles);

  // HiggsBackgroundPlan plan;
  // plan.runAnalyzer(inputFile, outputFile, outputEvery, numFiles);

  std::cout << "Processing complete!" << std::endl;
  std::cout << "Output written to " << outputFile << std::endl;

  return 0;
}

