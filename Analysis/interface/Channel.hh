#ifndef CHANNEL_HH
#define CHANNEL_HH

#include <unordered_map>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <string>
#include <vector>
#include <memory>

class Process;
class THStack;

class Channel
{
	public:

		//Channel(std::string name, std::string iYAxisName, std::vector<std::shared_ptr<Process>> iProcesses);
		Channel(std::string name, std::vector<std::shared_ptr<Process>> iProcesses);
		//Gets data from all processes in the format needed to make table
		std::vector<double> getYields(std::string dataType) const;
		std::vector<std::string> getNames() const;
		std::string getName() {return name;}
		std::vector<std::string> getNamesWithLabel(std::string label);
		//Makes stacked histogram
		THStack* getStack(std::string histType, std::string label = "", bool scaleToExpected = false) const;
		
		const std::shared_ptr<Process> findProcess(std::string processName) const;
		
		void labelProcess(std::string label, std::string processName);
		
		void labelProcess(std::string label, std::shared_ptr<Process> process);

		void addProcessLabel(std::string label, std::vector<std::shared_ptr<Process>> processes);

		std::vector<std::shared_ptr<Process>> getProcesses() {return processes;}

	private: 
		std::string name;
		//std::string yAxisName;
		std::vector<std::shared_ptr<Process>> processes;
		std::unordered_map<std::string, std::vector<std::shared_ptr<Process>>> map;
};


#endif
