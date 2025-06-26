#include "CMSAnalysis/Plans/interface/ModuleOptions.hh"
#include <fstream>


ModuleOptions::ModuleOptions()
{}

void ModuleOptions::setupOptions(std::string name)
{
	std::ifstream file;
	file.open(name);
	if (!file) 
	{
		throw std::runtime_error("Cannot open " + name);
	}

	std::string line;
	while (getline(file, line)) 
	{
		if (line.empty()) // Go to the next line if the current line is empty
    	{
      		continue;
    	}
    	else if (line.find('#') == std::string::npos) // If it doesn't have have a hashtag it is an option
    	{
			auto index = line.find(':');
			if (index != std::string::npos) //checks for colon
			{
				auto setting = line.substr(0, index);
				auto value = line.substr(index + 1, line.length());
				double number = std::stod(value);
				options.insert({setting,number});
			}
      	continue;
    	}
	}
}



