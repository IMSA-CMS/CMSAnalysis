#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <chrono>

#include "TFile.h"
#include "TH1.h"

#include "CMSAnalysis/Plans/interface/ModuleCollection.hh"

//trying to get rid of these
//#include "CMSAnalysis/Modules/interface/AnalysisModule.hh"
//#include "CMSAnalysis/Modules/interface/FilterModule.hh"
//#include "CMSAnalysis/Modules/interface/ProductionModule.hh"


#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "CMSAnalysis/Modules/interface/Module.hh"
#include "CMSAnalysis/Utility/interface/TDisplayText.h"
#include "CMSAnalysis/DataCollection/interface/ProcessDictionary.hh"
#include "CMSAnalysis/Modules/interface/AnalyzerEventInput.hh"

Analyzer::Analyzer() : 
eventInterface(nullptr),
input(new AnalyzerEventInput(&eventInterface))
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
  //std::cout << "There are " << analysisModules.size() << " analysis modules\n";
  for (auto module : analysisModules)
  {
    // Write the output
    module->doneProcessing();
    //std::cout << "FilterModules size : " << filterModules.size() << "\n";
    // if (filterModules.size() != 0)
    if (true)
    {
      //std::cout << "Finalizing analysis module: " << module->getFilter() << "\n";
      module->finalize();
      for (auto &str : filterNames) //writes analysis modules by filter string
      {
        auto it = filterDirectories.find(str);
        if (it == filterDirectories.end())
        {
          filterDirectories.insert({str,outputRootFile->mkdir((str + "_hists").c_str())});
        }
        filterDirectories[str]->cd();
        module->setFilterString(str);
        module->finalizeFilterString();
        outputRootFile->cd();
      }
    } else {
      module->setFilterString("");
      module->finalize();
    }
  }

  // Write total number of events
  auto eventsText = new TDisplayText(std::to_string(numOfEvents).c_str());
  eventsText->Write("NEvents");
  // Clean up
  outputRootFile->Close();
  delete outputRootFile;
}

//gets modules from module collection
void Analyzer::addModules(ModuleCollection modules)
{
  if (modules.getProductionModules().size() == 0)
  {
    std::cout << "You have no production modules!" << std::endl;
  }
  if (modules.getFilterModules().size() == 0)
  {
    std::cout << "You have no filter modules!" << std::endl;
  }
  if (modules.getAnalysisModules().size() == 0)
  {
    std::cout << "You have no analysis modules!" << std::endl;
  }
    productionModules = modules.getProductionModules();
    filterModules = modules.getFilterModules();
    analysisModules = modules.getAnalysisModules();
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

  // Checks if all dependencies are loaded properly
  for (auto module : getAllModules())
  {
    if (!checkModuleDependencies(module))
    {
      std::cout << typeid(*module).name() << "'s dependencies have not been loaded properly!\n";
    }
  }

  std::cout << "Finished checking module dependencies\n";

  // Initialize all modules
  for (auto module : getAllModules())
  {
    if (!module->getInput())
    {
      module->setInput(input);
    }
    module->initialize();
  }
}

void Analyzer::processOneEvent(const EventInterface *eInterface)
{
      eventInterface = eInterface;
      numOfEvents++;

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
        filterString += "_";
      }

      // Processes event through analysis modules
      if (continueProcessing)
      {

        filterNames.insert(filterString);

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

void Analyzer::addProductionModule(std::shared_ptr<ProductionModule> module)
{
  productionModules.push_back(module);
}

void Analyzer::addFilterModule(std::shared_ptr<FilterModule> module)
{
  filterModules.push_back(module);
}
void Analyzer::addAnalysisModule(std::shared_ptr<AnalysisModule> module)
{
  analysisModules.push_back(module);
}

bool Analyzer::checkModuleDependencies(std::shared_ptr<Module> module)
{
  auto modules = getAllModules();
  auto dependencies = module->getDependencies();
  for (auto modulePtr : dependencies)
  {
    if (std::find(modules.begin(), modules.end(), modulePtr) == modules.end())
    {
      return false;
    } 
  }

  return true;
}
