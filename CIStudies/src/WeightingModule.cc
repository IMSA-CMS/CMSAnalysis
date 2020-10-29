#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/FileParams.hh"

#include <fstream>
#include <boost/algorithm/string.hpp>

WeightingModule::WeightingModule()
{
  std::ifstream weightsFile("../bin/defs.py");

  if (!weightsFile)
    {
      throw std::runtime_error("Weights File not Found!");
    }

  std::cout << "About to print out defs.py" << std::endl;

  while (weightsFile)
    {
      std::string line;
      std::getline(weightsFile, line);

      auto colon = line.find(":");
      auto secondQuote = line.find("\"", 1);
      
      if (colon != std::string::npos && line.substr(0, 1) != "#")
	{
	  auto key = line.substr(1, secondQuote - 1);
	  double value = 0;

	  try 
	    {
	      value = stod(line.substr(colon + 1, line.length() - 1));
	    }
	  catch (...)
	    {
	      continue;
	    }

	  boost::to_lower(key);
	  weights[key] = value;
	}
    }
}

bool WeightingModule::process(const edm::EventBase& event)
{
  //std::cerr << "ENTERING WeightingModule" << std::endl;
  auto keyname = getFileParams().getFileKey();
  findWeight(keyname);

  //std::cerr << "EXITING WeightingModule" << std::endl;
  return true;
}

void WeightingModule::findWeight(std::string key)
{
  auto iterator = weights.find(key);
  if (iterator == weights.end())
    throw std::runtime_error("Key " + key + " not found!");
  weight = iterator->second;  
}
