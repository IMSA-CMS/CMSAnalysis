#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

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
#include "CMSAnalysis/DataCollection/interface/EventLoaderInputModule.hh"

Analyzer::Analyzer() :
  eventLoader(),
  input(new EventLoaderInputModule(&eventLoader))
  {
    dictionary.loadProcesses("textfiles/processes.txt");
  }
Analyzer::Analyzer(const Analyzer& analyzer) {
  eventLoader = analyzer.eventLoader;
  input = analyzer.input;
}
Analyzer::~Analyzer()
{
  delete input;
}

void Analyzer::run(const std::string& configFile, const std::string& outputFile, int outputEvery, int nFiles)
{
  setupFiles(configFile);
  
  process(outputEvery, nFiles);
  
  finalize(outputFile);
}

void Analyzer::setupFiles(const std::string& configFile)
{
  auto localSwitch = configFile.find(".root");
  if (localSwitch != std::string::npos)
  {
    files.push_back(configFile);
  } 
  else 
  {
     auto fileparams = dictionary.readFile(configFile);

    for (auto& filepar : fileparams)
    {
      // Get a list of Root files
      auto tempFiles = filepar.getFileList();
      files.insert(files.end(), tempFiles.begin(), tempFiles.end());
    }
    for (auto& filename : files)
    {
    const std::string eossrc = "root://cmsxrootd.fnal.gov//";
	  filename = eossrc + filename;
    }
  }
  std::cout << "# of root files: " << files.size() << std::endl;
}

void Analyzer::process(int outputEvery, int nFiles)
{
  // This keeps the histograms separate from the files they came from, avoiding much silliness
  TH1::AddDirectory(kFALSE);
  TH1::SetDefaultSumw2(kTRUE);

  // Get a list of FileParams objects
  eventLoader.setOutputEvery(outputEvery);

    // Initialize all modules
  for (auto module : getAllModules())
  {
    module->setInput(input);
    module->initialize();
  }
  int fileCounter = 0;
  for (auto& filename : files)
  {
    ++fileCounter;
    TFile* file = TFile::Open(filename.c_str(), "READ");
	  if (!file)
	  {
	   std::cout << "File " << filename << " not found!\n";
	   continue;
    }
    eventLoader.changeFile(file);
    while(true)
    {
      if (eventLoader.getFile()->isDone())
      {
        break;
      }
      ++numOfEvents;
      // eventLoader.getLeptons();
      bool continueProcessing = true;
      for (auto module : productionModules)
      {
        if (!module->processEvent())
        {
          continueProcessing = false;
          break;
        }
      }
      std::string filterString;
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
      if (continueProcessing)
      {
        filterNames.insert(filterString);
        for (auto module : analysisModules)
        {
          module->setFilterString(filterString);
          module->processEvent();
        }
      }
      eventLoader.getFile()->nextEvent();
    }
	  delete file;

    if (nFiles != -1 && fileCounter >= nFiles)
    {
      break;
    }
  }
  std::cout << "Events Processed: " << numOfEvents << std::endl;
}

void Analyzer::finalize(const std::string& outputFile)
{
   // Create the output file
  TFile* outputRootFile = new TFile(outputFile.c_str(), "RECREATE");

  // Finalize the modules
  for (auto module : productionModules)
  {
    module->finalize();
  }
  for (auto module : filterModules)
  {
    module->finalize();
  }
 
  // Finalize separately for each filterString, to be safe
  for (auto module : analysisModules)
  {
    module->doneProcessing();

    for (auto& str : filterNames)
	  {
	    module->setFilterString(str);
	    module->finalize();
	  }

      // Write the output
      module->writeAll();
  }

  // Write total number of events
  auto eventsText = new TDisplayText(std::to_string(numOfEvents).c_str());
  eventsText->Write("NEvents");

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
      //std::cout<<"Filter"<<mod<<std::endl;
      modules.push_back(mod);
    }
  for (auto mod : analysisModules)
    {
      modules.push_back(mod);
    }
  return modules;
}
