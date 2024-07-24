#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include "boost/algorithm/string.hpp"

//For some reason the .txt files (for now) have to be included in src folder for CrossSectionReader to see them
CrossSectionReader::CrossSectionReader(std::string fileAddress)
{
	std::ifstream weightsFile(fileAddress);

  if (!weightsFile)
    {
      throw std::runtime_error("Weights File not Found!"); // hello wooaojo
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
      std::getline(weightsFile, line);
      if (line == "")
      {
        continue;
      }
      std::istringstream str(line);
      str >> key >> value;
      boost::to_lower(key);
      weights[key] = value;
    }
  }
  weightsFile.close();
}

double CrossSectionReader::getCrossSection(std::string key) const
{
  boost::to_lower(key);
	auto iterator = weights.find(key);
	if (iterator == weights.end())
	{
		throw std::runtime_error("Key " + key + " not found!\n");
	}
	
	else
	{
	return iterator->second;  
	}
}