#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include "TFile.h"
#include "TH1.h"

#include "CMSAnalysis/DataCollection/interface/AnalysisModule.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/FilterModule.hh"
#include "CMSAnalysis/DataCollection/interface/Module.hh"
#include "CMSAnalysis/DataCollection/interface/ProductionModule.hh"
#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/Framework/interface/Event.h"

Analyzer::Analyzer() : eventLoader(), input(&eventLoader) {}

void Analyzer::run(const std::string &configFile, const std::string &outputFile,
                   int outputEvery, int nFiles) {
  // This keeps the histograms separate from the files they came from, avoiding
  // much silliness
  TH1::AddDirectory(kFALSE);
  TH1::SetDefaultSumw2(kTRUE);

  // Get a list of FileParams objects
  auto fileparams = inputFiles(configFile);
  eventLoader.setOutputEvery(outputEvery);

  // Initialize all modules
  for (auto module : getAllModules()) {
    module->setInput(&input);
    module->initialize();
  }

  // A list of all the filters we used
  std::unordered_set<std::string> filterNames;

  int numOfEvents = 0;

  for (auto &filepar : fileparams) {
    // Set the static FileParams object so the modules know the file parameters
    Module::setFileParams(filepar);

    // Get a list of Root files
    auto files = filepar.fileVector();
    std::cout << "# of root files: " << files.size() << std::endl;

    int fileCounter = 0;

    for (auto &filename : files) {
      fileCounter += 1;

      // Open files with rootxd
      const std::string eossrc = "root://cmsxrootd.fnal.gov//";

      std::string fileStr = eossrc + filename;
      TFile* file = TFile::Open(fileStr.c_str(), "READ");
      if (!file)
      {
        std::cout << "File " << fileStr << " not found!\n";
        continue;
      }
    
      eventLoader.changeFile(file);

      while(true)
      {
        if (eventLoader.getFile()->isDone())
        {
          break;
        }
      }
      std::cout << "Events Processed: " << numOfEvents << std::endl;
    }

    // Finalize the modules
    for (auto module : productionModules) {
      module->finalize();
    }
    for (auto module : filterModules) {
      module->finalize();
    }
    TFile *outputRootFile = new TFile(outputFile.c_str(), "RECREATE");

    // Finalize separately for each filterString, to be safe
    for (auto module : analysisModules) {
      module->doneProcessing();

      for (auto &str : filterNames) {
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
}

std::vector<std::string> Analyzer::parseLine(std::ifstream &txtFile) const {
  std::string line;
  std::getline(txtFile, line);

  // seperates each line by whitespace (tabs)
  std::istringstream stream(line);

  std::vector<std::string> category;

  int counter = 0;
  while (stream) {
    std::string word;
    stream >> word;

    // occasionally added empty strings to the vector which caused a runtime
    // error
    if (counter > 0 and word.size() > 0) {
      category.push_back(word);
    }
    ++counter;
  }

  return category;
}

std::vector<FileParams> Analyzer::inputFiles(const std::string &txtFile) const {
  std::ifstream inputFiles(txtFile);

  if (!inputFiles) {
    throw std::runtime_error("File " + txtFile + " not found!");
  }

  // each vector contains the options selected in "pickFiles.txt"
  //  Configuration file must be in this order
  auto process = parseLine(inputFiles);
  auto year = parseLine(inputFiles);
  auto lepton = parseLine(inputFiles);
  auto mass = parseLine(inputFiles);
  auto lambda = parseLine(inputFiles);
  auto interference = parseLine(inputFiles);
  auto helicity = parseLine(inputFiles);

  std::vector<FileParams> params;

  // adds all of the files to a larger vector that is seperated by mass cuts
  for (auto &processStr : process)
    for (auto &yearStr : year) {
      for (auto &leptonStr : lepton) {
        for (auto &lambdaStr : lambda) {
          for (auto &interferenceStr : interference) {
            for (auto &helicityStr : helicity) {
              for (auto &massStr : mass) {
                // based on the options selected, it adds the respective files
                // following the standard naming system
                params.push_back(FileParams(processStr, yearStr, helicityStr,
                                            interferenceStr, massStr, lambdaStr,
                                            leptonStr));
              }
            }
          }
        }
      }
    }

  std::cout << params[0].getFileKey() << std::endl;
  return params;
}

std::vector<std::shared_ptr<Module>> Analyzer::getAllModules() const {
  std::vector<std::shared_ptr<Module>> modules;
  for (auto mod : productionModules) {
    modules.push_back(mod);
  }
  for (auto mod : filterModules) {
    modules.push_back(mod);
  }
  for (auto mod : analysisModules) {
    modules.push_back(mod);
  }
  return modules;
}
