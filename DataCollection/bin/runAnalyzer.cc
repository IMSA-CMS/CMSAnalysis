#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/IDType.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"
#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/AnalyzerOptions.hh"
#include "CMSAnalysis/DataCollection/interface/ModuleOptions.hh"
// #include "CMSAnalysis/DataCollection/bin/massResolutionAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/HPlusPlusMassAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/leptonJetReconstructionAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/displacedVertexAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/massAcceptanceAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/triggerAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/invariantMassAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/HiggsBackgroundAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/LeptonJetBackgroundAnalysis.cc"
// #include "CMSAnalysis/DataCollection/bin/FilestripAnalysis.cc"

int main(int argc, char **argv)
{
  std::string particleDatabase("ParticleData.txt");
  if (ParticleType::loadParticleDatabase(particleDatabase))
  {
    std::cout << particleDatabase << " has been loaded properly!\n";
  }
  else
  {
    std::cout << particleDatabase << " has not been loaded properly!\n";
  }


  auto start = std::chrono::steady_clock::now();

  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser("Analyze FWLite Histograms");

  parser.addOption("output", optutl::CommandLineParser::kString, "Particle", "");

  parser.addOption("input", optutl::CommandLineParser::kString, "Input", "");
  parser.addOption("numFiles", optutl::CommandLineParser::kInteger, "Number of Files", -1); // Change last input to -1 later

  parser.addOption("analysis", optutl::CommandLineParser::kString, "Type of Analysis", "");
  parser.addOption("moduleOptions", optutl::CommandLineParser::kString, "Module Specific Options", "");
  parser.parseArguments(argc, argv);

  std::string inputFile = parser.stringValue("input");

  std::string outputFile = parser.stringValue("output");
  int numFiles = parser.integerValue("numFiles");
  std::string analysisType = parser.stringValue("analysis");

  std::string moduleOptionsFile = parser.stringValue("moduleOptions");

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

  unsigned maxEvents = parser.integerValue("maxEvents");

  //   Selection of data collection plan has moved to command line argument "analysis"
  //   The key for each Plan can now be found in AnalyzerOptions.cc

  //   e.g. runAnalyzer input=input.txt output=output.root analysis=DisplacedVertex

  std::string analysis = options.checkSelectedAnalysis(analysisType);
  std::cout << "Reading input file " << inputFile << std::endl;
  // analyzer.run(inputFile, outputFile, outputEvery, numFiles);

  // analysisPlans[analysisType]->runAnalyzer(inputFile, outputFile, outputEvery, numFiles);

  DataCollectionPlan *plan = options.getAnalysisPlans().at(analysis);

  auto moduleOptionsPtr = std::make_shared<ModuleOptions>();
  if (!moduleOptionsFile.empty())
  {
    moduleOptionsPtr->setupOptions(moduleOptionsFile);
    plan->getOptions(moduleOptionsPtr);
  }

  plan->initialize();
  plan->runEventLoader(inputFile, outputFile, outputEvery, numFiles, maxEvents);

  std::cout << "Processing complete!" << std::endl;
  std::cout << "Output written to " << outputFile << std::endl;

  auto end = std::chrono::steady_clock::now();

  std::chrono::duration<double> processingTime = end - start;

  std::cout<<"Processing time: "<<processingTime.count()<<"s"<<std::endl;
  return 0;
}