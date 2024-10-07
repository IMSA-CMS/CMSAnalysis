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
#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/AnalyzerOptions.hh"
#include "CMSAnalysis/Plans/interface/ModuleOptions.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"

int main(int argc, char **argv)
{
  auto start = std::chrono::steady_clock::now();

  std::string particleDatabase(Utility::getFullPath("ParticleData.txt"));
  if (ParticleType::loadParticleDatabase(particleDatabase))
  {
    std::cout << particleDatabase << " has been loaded properly!\n";
  }
  else
  {
    std::cout << particleDatabase << " has not been loaded properly!\n";
  }

  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser("Analyze FWLite Histograms");

  parser.addOption("output", optutl::CommandLineParser::kString, "Particle", "");

  parser.addOption("input", optutl::CommandLineParser::kString, "Input", "");
  parser.addOption("numFiles", optutl::CommandLineParser::kInteger, "Number of Files", -1); // Change last input to -1 later
  parser.addOption("skipFiles", optutl::CommandLineParser::kInteger, "Number of Files to Skip Before  Starting: ", 0);


  parser.addOption("analysis", optutl::CommandLineParser::kString, "Type of Analysis", "");
  parser.addOption("moduleOptions", optutl::CommandLineParser::kString, "Module Specific Options", "");

//  parser.addOption("maxEvents", optutl::CommandLineParser::kInteger, "Number of events to process", -1);
  parser.parseArguments(argc, argv);

  std::string inputFile = parser.stringValue("input");

  std::string outputFile = parser.stringValue("output");
  int numFiles = parser.integerValue("numFiles");
  int skipFiles = parser.integerValue("skipFiles");

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
  // modules.run(inputFile, outputFile, outputEvery, numFiles);

  // analysisPlans[analysisType]->runAnalyzer(inputFile, outputFile, outputEvery, numFiles);

  DataCollectionPlan *plan = options.getAnalysisPlans().at(analysis);

  auto moduleOptionsPtr = std::make_shared<ModuleOptions>();
  if (!moduleOptionsFile.empty())
  {
    moduleOptionsPtr->setupOptions(moduleOptionsFile);
    plan->getOptions(moduleOptionsPtr);
  }

  plan->initialize();   
  Analyzer analyzer;
  auto modules = plan->getModules();
  analyzer.addModules(modules);
   EventLoader eventLoader(EventLoader::fetchRootFiles(inputFile), &analyzer);
    analyzer.initialize(Utility::getBasePath()+"Output/", outputFile);
    eventLoader.run(outputEvery, numFiles, maxEvents, skipFiles);
    analyzer.writeOutputFile();

  
  std::cout << "Processing complete!" << std::endl;
  std::cout << "Output written to " << outputFile << std::endl;

  auto end = std::chrono::steady_clock::now();

  std::chrono::duration<double> processingTime = end - start;

  std::cout << "Processing time: "<<processingTime.count()<<"s"<<std::endl;
  return 0;
}