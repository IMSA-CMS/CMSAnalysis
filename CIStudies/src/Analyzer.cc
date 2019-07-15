#include "CIAnalysis/CIStudies/interface/Analyzer.hh"

#include <fstream>
#include <iostream>
#include <sstream>

#include "TFile.h"

#include "CIAnalysis/CIStudies/interface/AnalysisModule.hh"
#include "CIAnalysis/CIStudies/interface/ProductionModule.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"

void Analyzer::run(const std::string& configFile, const std::string& outputFile, int outputEvery)
{
  std::string particle1;
  auto filenames = inputFiles(configFile, particle1);
  auto files = getFiles(filenames);

  TFile *of = new TFile(outputFile.c_str(), "recreate");

  for (auto module : analysisModules)
    {
      module->initialize();
    }

  for (auto& massFiles : files)
    {
      std::cout << "# of root files: " << massFiles.size() << std::endl;

      for (auto& filename : massFiles)
	{
	  const std::string eossrc = "root://cmsxrootd.fnal.gov//";

	  std::string fileStr = eossrc + filename;
	  TFile* file = TFile::Open(fileStr.c_str(), "READ");
	  if (!file)
	    {
	      std::cout << "File " << fileStr << " not found!\n";
	      continue;
	    } 

	  fwlite::Event ev(file);

	  std::cerr << "Events: " << ev.size() << std::endl;
      
	  int ievt = 0;
	  for (ev.toBegin(); !ev.atEnd(); ++ev, ++ievt)
	    {
	      const edm::EventBase& event = ev;

	      if (outputEvery != 0 && ievt > 0 && ievt % outputEvery == 0) 
		std::cout << "Processing event: " << ievt << std::endl; 

	      for (auto module : productionModules)
		{
		  module->process(event);
		}
	      for (auto module : analysisModules)
		{
		  module->process(event);
		}
	    }

	  delete file;
	}
      
    }

  of->cd();
  for (auto module : analysisModules)
    {
      module->writeAllHistograms();
    }
  of->Close();
}

//fills each of the inner vectors with the info in their respective lines in "pickFiles.txt"
std::vector<std::string> Analyzer::parseLine(std::ifstream& txtFile)
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

std::vector<std::vector<std::string>> Analyzer::inputFiles(const std::string& txtFile, std::string& particle1)
{
  //inputFiles looks at "pickFiles.txt" to determine which data (lepton type, mass cuts, etc.)  is inputed to make the histograms
  std::ifstream inputFiles(txtFile);

  if (!inputFiles)
    {
      throw std::runtime_error("File " + txtFile + " not found!");
    }
  
  //each vector contains the options selected in "pickFiles.txt"
  auto year = parseLine(inputFiles);
  auto lepton = parseLine(inputFiles);

  //particle1 is set based on the lepton chosen in "pickFiles,txt"
  if (lepton[0] == "E")
    {
      particle1 = "electron";
    }
  else if (lepton[0] == "Mu")
    {
      particle1 = "muon";
    }
  else 
    {
      throw std::runtime_error("Particle type " + lepton[0] + " not valid! In  \"pickFiles.txt\", you shoukd include either E or Mu to note the particle type.");
    }

  auto mass = parseLine(inputFiles);

  //the files in the 2017 directory have a different naming convention so I correct for it
  if (year[0] == "2017")
    {
      for (auto& massStr : mass)
	{
	  if (massStr == "300")
	    {
	      massStr = "300to800";
	    }
	  else if (massStr == "800")
	    {
	      massStr = "800to1300";
	    }
	  else if (massStr == "1300")
	    {
	      massStr = "1300to2000";
	    }
	  else if (massStr == "2000")
	    {
	      massStr = "2000toInf";
	    }
	  else if (massStr != "")
	    {
	      throw std::runtime_error("Mass input " + massStr + " not valid! Your options include 300, 800, 1300, and 2000.");
	    }
	}
    }

  auto lambda = parseLine(inputFiles);
  auto interference = parseLine(inputFiles);
  auto helicity = parseLine(inputFiles);

  std::vector<std::vector<std::string>> massCuts;

  //adds all of the files to a larger vector that is seperated by mass cuts
  for (auto& massStr : mass)
    {
      std::vector<std::string> sameMass;
      
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
			  std::string file = "textfiles/" + yearStr + "/CITo2" + leptonStr + "M" + massStr + "_" + "Lam" +  lambdaStr + interferenceStr + helicityStr +  ".txt";
			  
			  sameMass.push_back(file);
			}
		    }
		}
	    }
	}

      massCuts.push_back(sameMass);
    }

  return massCuts;
}

std::vector<std::vector<std::string>> Analyzer::getFiles(const std::vector<std::vector<std::string>>& inputs)
{
  std::vector<std::vector<std::string>> array(inputs.size());
  
  //create 2D array of mass ranges and different types
  for (size_t i = 0; i < inputs.size(); ++i)
    {
      for (auto& filename : inputs[i])
	{
	  std::ifstream ifs(filename);

	  if (!ifs)
	    throw std::runtime_error("File " + filename + " not found! Refer to the file, \"README.txt\", in the CIAnalysis folder to make sure the inputs added into \"pickFiles.txt\" are valid.");

	  std::string w_content;
	  while (std::getline(ifs, w_content))
	    {
	      if (!w_content.empty())
		{
		  array[i].push_back(w_content);
		}
	    }
	}
    }
  return array;
}
