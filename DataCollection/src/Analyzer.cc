#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <chrono>

#include "TFile.h"
#include "TH1.h"

#include "CMSAnalysis/DataCollection/interface/AnalysisModule.hh"
#include "CMSAnalysis/DataCollection/interface/FilterModule.hh"
#include "CMSAnalysis/DataCollection/interface/ProductionModule.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "CMSAnalysis/DataCollection/interface/Module.hh"
#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"
#include "CMSAnalysis/DataCollection/interface/ProcessDictionary.hh"
#include "CMSAnalysis/DataCollection/interface/AnalyzerInputModule.hh"

Analyzer::Analyzer() : 
eventInterface(nullptr),
input(new AnalyzerInputModule(&eventInterface))
{
}

Analyzer::Analyzer(const Analyzer &analyzer)
{

  input = analyzer.input;
}

Analyzer::~Analyzer()
{
  delete input;
}


void Analyzer::writeOutputFile(const std::string &outputFile)
{
  // Create the output file
  TFile *outputRootFile = new TFile(outputFile.c_str(), "RECREATE");
  // Finalize the modules
  for (auto module : productionModules)
  {
    module->finalize();
  }
  for (auto module : filterModules)
  {
    module->finalize();
  }

  outputRootFile->cd();
  //Finalize separately for each filterString, to be safe
  for (auto module : analysisModules)
  {
       // Write the output
    module->doneProcessing();
    if (filterModules.size() != 0)
    {
      for (auto &str : filterNames)
      {
        auto it = filterDirectories.find(str);
        if (it == filterDirectories.end())
        {
          filterDirectories.insert({str,outputRootFile->mkdir((str + "_hists").c_str())});
        }
        filterDirectories[str]->cd();
        module->setFilterString(str);
        module->finalize();
        module->writeAll();
        outputRootFile->cd();
      }
    } else {
      module->setFilterString("");
      module->finalize();
      module->writeAll();
    }
  }

  // Write total number of events
  auto eventsText = new TDisplayText(std::to_string(numOfEvents).c_str());
  eventsText->Write("NEvents");
  auto eventsText1 = new TDisplayText(std::to_string(numOfEvents124).c_str());
  eventsText1->Write("NEvents124");
  auto eventsText2 = new TDisplayText(std::to_string(numOfEvents137).c_str());
  eventsText2->Write("NEvents137");
  // Clean up
  outputRootFile->Close();
  delete outputRootFile;
}

std::vector<std::shared_ptr<Module>> Analyzer::getAllModules() const
{
  std::vector<std::shared_ptr<Module>> modules;
  for (auto mod : productionModules)
  {
    modules.push_back(mod);
  }
  for (auto mod : filterModules)
  {
    modules.push_back(mod);
  }
  for (auto mod : analysisModules)
  {
    modules.push_back(mod);
  }
  return modules;
}

void Analyzer::initialize()
{
  // This keeps the histograms separate from the files they came from, avoiding errors
  TH1::AddDirectory(kFALSE);
  TH1::SetDefaultSumw2(kTRUE);

  // Initialize all modules
  for (auto module : getAllModules())
  {
    module->setInput(input);
    module->initialize();
  }
}

void Analyzer::processOneEvent(const EventInterface *eInterface)
{
      eventInterface = eInterface;

      bool continueProcessing = true;
      std::string filterString;
      // Processes event through production modules
      for (auto module : productionModules)
      {
        if (!module->processEvent())
        {
          continueProcessing = false;
          break;
        }

      }
      numOfEvents124++;

      // Processes event through filter modules
      for (auto module : filterModules)
      {
        if (!module->processEvent())
        {
          continueProcessing = false;
          break;
        }
        else
        {
          filterString += module->getFilterString();
        }
      }
      numOfEvents137++;

      // Processes event through analysis modules
      if (continueProcessing)
      {

        filterNames.insert(filterString);
        numOfEvents++;

        for (auto module : analysisModules)
        {
          module->setFilterString(filterString);
          if (!module->processEvent())
          {
            continueProcessing = false;
            break;
          }
        }
      }

}

