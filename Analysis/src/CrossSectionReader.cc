#include "CMSAnalysis/Analysis/interface/CrossSectionReader.hh"

CrossSectionReader::CrossSectionReader(std::string fileAddress)
{
	std::ifstream weightsFile(fileAddress);

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
      std::getline(weightsFile, line);
      std::istringstream str(line);
      str>>key>>value;
      boost::to_lower(key);
      //std::cout << "key: " + key + " " + "value: " + std::to_string(value) + "\n";
      weights[key] = value;
    }
  }
  weightsFile.close();
}

double CrossSectionReader::getWeight(std::string key) const
{
	auto iterator = weights.find(key);
	if (iterator == weights.end())
	{
		//std::cerr << "Key " << key << " not found! " << std::endl;
		//weight = 0;
	
		throw std::runtime_error("Key " + key + " not found!");
	}
	
	else
	{
	return iterator->second;  
	}
}