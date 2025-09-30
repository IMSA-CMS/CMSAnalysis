#ifndef CHANNEL_HH
#define CHANNEL_HH

#include <unordered_map>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <string>
#include <vector>
#include <memory>
#include "TH1.h"
#include "Process.hh"

class Process;
class THStack;
class Systematic;
class Channel
{
	public:

		//Channel(std::string name, std::string iYAxisName, std::vector<std::shared_ptr<Process>> iProcesses);
		Channel(std::string name, std::vector<std::shared_ptr<Process>> iProcesses);
		//Gets data from all processes in the format needed to make table
		std::vector<double> getYields(HistVariable dataType) const;
		std::vector<std::string> getNames() const;
		std::string getName() {return name;}
		std::vector<std::string> getNamesWithLabel(std::string label);
		//Makes stacked histogram
		THStack* getStack(HistVariable histType, std::string label = "", bool scaleToExpected = false, int rebinConstant = 1) const;
		
		std::vector<TH1*> getHists(HistVariable histType, std::string label = "", bool scaleToExpected = false) const;

		const std::shared_ptr<Process> findProcess(std::string processName) const;
		
		void labelProcess(std::string label, std::string processName);
		
		void labelProcess(std::string label, std::shared_ptr<Process> process);

		void addProcessLabel(std::string label, std::vector<std::shared_ptr<Process>> processes);

		void makeDatacard(HistVariable histType);
		
		void CombineDatacard(HistVariable histType);

		void addGlobalSystematic(Systematic& systematic);

		std::vector<std::shared_ptr<Process>> getProcesses() {return processes;}

		static TH1* combineHists(const std::vector<TH1*>& hists);


	private: 
		std::string name;
		//std::string yAxisName;
		std::vector<std::shared_ptr<Process>> processes;
		std::unordered_map<std::string, std::vector<std::shared_ptr<Process>>> map;
};


#endif
