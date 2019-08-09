#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include "TFile.h"
#include "TH1.h"

#include "CIAnalysis/CIStudies/interface/AnalysisModule.hh"
#include "CIAnalysis/CIStudies/interface/FilterModule.hh"
#include "CIAnalysis/CIStudies/interface/ProductionModule.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "CIAnalysis/CIStudies/interface/Module.hh"

void Analyzer::run(const std::string& configFile, const std::string& outputFile, int outputEvery)
{
  // This keeps the histograms separate from the files they came from, avoiding much silliness
  TH1::AddDirectory(kFALSE);

  // Get a list of FileParams objects
  auto fileparams = inputFiles(configFile);

  // Initialize all modules
  for (auto module : getAllModules())
    {
      module->initialize();
    }

  // A list of all the filters we used
  std::unordered_set<std::string> filterNames;

  for (auto& filepar : fileparams)
    {
      // Set the static FileParams object so the modules know the file parameters
      Module::setFileParams(filepar);

      // Get a list of Root files
      auto files = filepar.fileVector();
      std::cout << "# of root files: " << files.size() << std::endl;

      for (auto& filename : files)
	{
	  // Open files with rootxd
	  const std::string eossrc = "root://cmsxrootd.fnal.gov//";

	  std::string fileStr = eossrc + filename;
	  TFile* file = TFile::Open(fileStr.c_str(), "READ");
	  if (!file)
	    {
	      std::cout << "File " << fileStr << " not found!\n";
	      continue;
	    } 

	  // Extract events
	  fwlite::Event ev(file);

	  std::cerr << "Events: " << ev.size() << std::endl;
      
	  int ievt = 0;

	  for (ev.toBegin(); !ev.atEnd(); ++ev, ++ievt)
	    {
	      const edm::EventBase& event = ev;

	      if (outputEvery != 0 && ievt > 0 && ievt % outputEvery == 0) 
		std::cout << "Processing event: " << ievt << std::endl; 

	      // Run all production modules
	      bool continueProcessing = true;
	      for (auto module : productionModules)
		{
		  if (!module->process(event))
		    {
		      continueProcessing = false;
		      break;
		    }
		}

	      // Run all filter modules and get filter string
	      std::string filterString;
	      for (auto module : filterModules)
		{
		  if (!module->process(event))
		    {
		      continueProcessing = false;
		      break;
		    }
		  else
		    {
		      filterString += module->getFilterString();
		    }
		}

	      if (!continueProcessing)
		continue;

	      // Keep track of the filters we are using (multiple insertions
	      // will cause no effect on a map)
	      filterNames.insert(filterString);

	      // Run all analysis modules
	      for (auto module : analysisModules)
		{
		  // Set the filter string first
		  module->setFilterString(filterString);
		  module->process(event);
		}
	    }

	  delete file;
	}
    }      
  
  // Finalize the modules
  for (auto module : productionModules)
    {
      module->finalize();
    }
  for (auto module : filterModules)
    {
      module->finalize();
    }
  TFile* outputRootFile = new TFile(outputFile.c_str(), "RECREATE");

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

  // Clean up
  outputRootFile->Close();

  delete outputRootFile;
}

std::vector<std::string> Analyzer::parseLine(std::ifstream& txtFile) const
{
  std::string line;
  std::getline(txtFile,line);
  
  //seperates each line by whitespace (tabs)
  std::istringstream stream(line);

  std::vector<std::string> category;

  int counter = 0; 
  while (stream)
    {
      std::string word;
      stream >> word;
      
      //occasionally added empty strings to the vector which caused a runtime error
      if (counter > 0 and word.size() > 0)
	{
	  category.push_back(word);
	}
      ++counter;
    }

  return category;
}

std::vector<FileParams> Analyzer::inputFiles(const std::string& txtFile) const
{
  std::ifstream inputFiles(txtFile);

  if (!inputFiles)
    {
      throw std::runtime_error("File " + txtFile + " not found!");
    }
  
  //each vector contains the options selected in "pickFiles.txt"
  // Configuration file must be in this order
  auto process = parseLine(inputFiles);
  auto year = parseLine(inputFiles);
  auto lepton = parseLine(inputFiles);
  auto mass = parseLine(inputFiles);
  auto lambda = parseLine(inputFiles);
  auto interference = parseLine(inputFiles);
  auto helicity = parseLine(inputFiles);

  std::vector<FileParams> params;

  //adds all of the files to a larger vector that is seperated by mass cuts
  for (auto& processStr : process)
    for (auto& massStr : mass)
      {
	for (auto& yearStr : year)
	  {
	    for (auto& leptonStr : lepton)
	      {
		for (auto& lambdaStr : lambda)
		  {
		    for (auto& interferenceStr : interference)
		      {
			for (auto& helicityStr : helicity)
			  {
			    //based on the options selected, it adds the respective files following the standard naming system
			    params.push_back(FileParams(processStr, yearStr, helicityStr, interferenceStr, massStr, 
							lambdaStr, leptonStr));
			  }
		      }
		  }
	      }
	  }
      }
      

  return params;
}

std::vector<Module*> Analyzer::getAllModules() const
{
  std::vector<Module*> modules;
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
