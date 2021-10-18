#include "CIAnalysis/CIStudies/interface/WeightingModule.hh"
#include "CIAnalysis/CIStudies/interface/FileParams.hh"

#include <fstream>
#include <boost/algorithm/string.hpp>

WeightingModule::WeightingModule()
{
  std::ifstream weightsFile("../bin/crossSections.txt");

  if (!weightsFile)
    {
      throw std::runtime_error("Weights File not Found!");
    }


  std::string line;
  std::string key;
  std::string valueString;
  double value = 0;
  while (weightsFile)
  { 
    if ((weightsFile.peek() == '#') || (weightsFile.peek() == '\n'))
    {
      std::getline(weightsFile, line);
    }
    else
    {
      std::getline(weightsFile, key, '\t');
      boost::to_lower(key);
      std::getline(weightsFile, valueString);  
      value = std::stod(valueString);
      //std::cout << "key: " + key + " " + "value: " + std::to_string(value) + "\n";
      weights[key] = value;
    }
  }
  weightsFile.close();
}
  
    /* {
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
    } */

bool WeightingModule::process()
{
  //std::cerr << "ENTERING WeightingModule" << std::endl;
  auto keyname = getFileParams().getFileKey();
  findWeight(keyname);

  //std::cerr << "EXITING WeightingModule" << std::endl;
  return true;
}

void WeightingModule::findWeight(std::string key)
{
  //std::cout << "Key: " << key << '\n';
  auto iterator = weights.find(key);
  if (iterator == weights.end())
  {
    std::cerr << "Key " << key << " not found! " << std::endl;
    weight = 0;
  
    //throw std::runtime_error("Key " + key + " not found!");
  }
  else if (key == "")
  {
    weight = 0;
  }
  else
  {
   weight = iterator->second;  
  }
}





