#ifndef MODULEOPTIONS_HH
#define MODULEOPTIONS_HH

#include <unordered_map>
#include <vector>
#include <string>

//Takes textfile input from runAnalyzer.cc and creates a map of options. 
//This map can directly be accessed in plans to configure any setting
class ModuleOptions
{
	public:
	ModuleOptions();
	
	//returns map of options
	std::unordered_map<std::string, double> getOptions() {return options;};

	//creates map from a file path
	void setupOptions(std::string name);

	private:
	std::unordered_map<std::string, double> options;
};

#endif