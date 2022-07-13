#include <iostream>

#include "TROOT.h"
#include "TSystem.h"

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

#include "CMSAnalysis/DataCollection/interface/DisplacedVertexPlan.hh"
#include "CMSAnalysis/DataCollection/interface/HiggsBackgroundPlan.hh"
#include "CMSAnalysis/DataCollection/interface/HPlusPlusMassPlan.hh"
#include "CMSAnalysis/DataCollection/interface/InvariantMassPlan.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetBackgroundPlan.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/MassAcceptancePlan.hh"
#include "CMSAnalysis/DataCollection/interface/MassResolutionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/TriggerPlan.hh"



int main(int argc, char **argv) {
  gROOT->SetBatch(true);
  gSystem->Load("libFWCoreFWLite");
  FWLiteEnabler::enable();
  gSystem->Load("libDataFormatsFWLite");

  optutl::CommandLineParser parser ("Analyze FWLite Histograms");

  parser.addOption("output", optutl::CommandLineParser::kString, "Particle", "");

  parser.addOption("input", optutl::CommandLineParser::kString, "Input", "");
  parser.addOption("numFiles", optutl::CommandLineParser::kInteger, "Number of Files", -1);

  parser.addOption("analysis", optutl::CommandLineParser::kString, "Type of Analysis", "");
  parser.parseArguments (argc, argv);

  std::string inputFile = parser.stringValue("input");

  std::string outputFile = parser.stringValue("output");
  int numFiles = parser.integerValue("numFiles");
  std::string analysisType = parser.stringValue("analysis");

  if (inputFile.empty())
  {
    inputFile = "textfiles/pickFiles.txt";
  }

  if (outputFile.empty())
  {
    outputFile = "default.root";
  }


  std::cout << "Reading input file " << inputFile << std::endl;

  unsigned outputEvery = parser.integerValue("outputEvery");

/* 
  Selection of data collection plan has moved to command line argument "analysis"
  The key for each Plan can be found in the insertions below
  
  e.g. runAnalyzer input=input.txt output=output.root analysis=DisplacedVertex
*/
  std::unordered_map<std::string, DataCollectionPlan*> analysisPlans;
  analysisPlans["DisplacedVertex"] = new DisplacedVertexPlan();
  analysisPlans["HiggsBackground"] = new HiggsBackgroundPlan();
  analysisPlans["InvariantMass"] = new InvariantMassPlan();
  analysisPlans["LeptonJetBackground"] = new LeptonJetBackgroundPlan();
  analysisPlans["MassResolution"] = new MassResolutionPlan();
  analysisPlans["LeptonJetReconstruction"] = new LeptonJetReconstructionPlan();
  analysisPlans["MassAcceptance"] = new MassAcceptancePlan();
  analysisPlans["Trigger"] = new TriggerPlan();

  if (analysisPlans.find(analysisType)==analysisPlans.end())
  {
    throw std::runtime_error("selected analysis does not exist");
  }

  analysisPlans[analysisType]->runAnalyzer(inputFile, outputFile, outputEvery, numFiles);
  
  /* HiggsBackgroundPlan plan;
  plan.runAnalyzer(inputFile, outputFile, outputEvery, numFiles); // */

  std::cout << "Processing complete!" << std::endl;
  std::cout << "Output written to " << outputFile << std::endl;

  return 0;
}
