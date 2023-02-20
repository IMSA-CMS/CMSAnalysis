#ifndef ANALYZEROPTIONS_HH
#define ANALYZEROPTIONS_HH

#include <unordered_map>
#include <vector>
#include <string>
#include "IDType.hh"

class DataCollectionPlan;

// A class that contains both the pickfile generating interface if a pickfile is not specified, 
// and an interface to select an analysis if one is not specified in the runAnalyzer command.
// Used in runAnalyzer.cc
class AnalyzerOptions
{
	public:
	// New plans should be added to this constructor so that they appear in the analysisPlans map.
	AnalyzerOptions();

	std::string pickfileInterface(); 

	// Reads processes.txt and returns a string with the names of the processes in the file
	std::vector<std::string> processNames();

	// gets user input for and checks the process and idtypes of the pickfile
	std::vector<std::pair<std::string, std::string>> getProcessAndIDTypes(std::vector<std::string> processes);

	// gets user input for the file name and calls the check function, returns the file name in a string
	std::string takeFilename();

	bool checkFilename(std::string name);

	// Print current status and ask for any changes
	void currentStatus(std::vector<std::pair<std::string, std::string>>* pickfileInfo, std::vector<std::string> processNames);

	// Converts the map to a pickfile
	void makePickfile(std::vector<std::pair<std::string, std::string>> info);

	// Fills the idtypes of the selected process
	std::vector<IDType> findProcessIDTypes(std::string process);

	// Takes a vector of strings and outputs a string with the contents of the vector tab separated
	std::string separateVector(std::vector<std::string> list, char separator);

	// Checks whether the selected item was in the list 
	bool checkInput(std::string input, std::vector<std::string> list);

	// A function that gets the user's input, adds a newline character and converts the string to lowercase, only for aesthetic purposes
	std::string takeInput();

	// A function that prompts for the user's input, and returns a pair with the user's input and corresponding category
	std::pair<std::string, std::string> promptInput(std::string name, std::vector<std::string> categories);

	// Takes the user's input and checks it. Only returns the user's input.
	std::string takeSingleInput(std::vector<std::string> categories, std::string promptString);

	// Checks if the selected analysis is in the map. If not, asks the user to enter an analysis.
	std::string checkSelectedAnalysis(std::string analysis);

	std::unordered_map<std::string, DataCollectionPlan*> getAnalysisPlans() const {return analysisPlans;} 

	private:
	std::unordered_map<std::string, DataCollectionPlan*> analysisPlans;
};

#endif

