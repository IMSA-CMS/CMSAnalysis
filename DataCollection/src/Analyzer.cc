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

Analyzer::Analyzer() : eventLoader(),
                       input(new EventLoaderInputModule(&eventLoader))
{
  dictionary.loadProcesses("textfiles/processes.txt");
}

Analyzer::Analyzer(const Analyzer &analyzer)
{
  eventLoader = analyzer.eventLoader;
  input = analyzer.input;
}

Analyzer::~Analyzer()
{
  delete input;
}

void Analyzer::run(const std::string &configFile, const std::string &outputFile, int outputEvery, int nFiles)
{
  fetchRootFiles(configFile);
  processRootFiles(outputEvery, nFiles);
  writeOutputFile(outputFile);
}

void Analyzer::fetchRootFiles(const std::string &configFile)
{
  auto substringFound = configFile.find(".root");
  bool isLocalFile = substringFound != std::string::npos;
  if (isLocalFile)
  {
    rootFiles.push_back(configFile);
  }
  else
  {
    auto fileparams = dictionary.readFile(configFile);
    for (auto &filepar : fileparams)
    {
      // Get a list of Root files for each filpar object
      auto tempFiles = filepar.getFileList();
      rootFiles.insert(rootFiles.end(), tempFiles.begin(), tempFiles.end());
    }
    for (auto &fileName : rootFiles)
    {
      // Adds prefix necessary to read remote files
      const std::string eossrc = "root://cmsxrootd.fnal.gov//";
      fileName = eossrc + fileName;
    }
  }
  std::cout << "# of root files: " << rootFiles.size() << std::endl;
}

void Analyzer::processRootFiles(int outputEvery, int nFiles)
{
  // This keeps the histograms separate from the files they came from, avoiding errors
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
  // Loops through every file
  int fileCounter = 0;
  for (auto &fileName : rootFiles)
  {
    ++fileCounter;
    TFile *file = TFile::Open(fileName.c_str(), "READ");
    if (!file)
    {
      std::cout << "File " << fileName << " not found!\n";
      continue;
    }
    eventLoader.changeFileFormat(file); // Makes a GenSimEventFile, DelphesEventFile or MiniAODFile shared pointer
    // Loops through every event in the file
    while (true)
    {
      if (eventLoader.getFile()->isDone())
      {
        numOfEvents += eventLoader.getFile()->getEventCount() - 1; //-1 is necessary to not count the last event which is invalid
        break;
      }
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
      eventLoader.getFile()->nextEvent();
    }
    delete file;
    // Checks that the correct number of files are processed
    if (nFiles != -1 && fileCounter >= nFiles)
    {
      break;
    }
  }
  std::cout << "Events Processed: " << numOfEvents << std::endl;
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
  // Finalize separately for each filterString, to be safe
  for (auto module : analysisModules)
  {
    module->doneProcessing();
    for (auto &str : filterNames)
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
    modules.push_back(mod);
  }
  for (auto mod : analysisModules)
  {
    modules.push_back(mod);
  }
  return modules;
}
