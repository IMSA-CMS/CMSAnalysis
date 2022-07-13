#ifndef CHANNEL_HH
#define CHANNEL_HH

#include <unordered_map>

#include <string>
#include <vector>
#include <memory>

class Process;
class THStack;

class Channel
{
	public:
		Channel(std::string name, std::vector<std::shared_ptr<Process>> iProcesses);
		
		THStack* getStack(std::string label = "", bool scaleToExpected = false) const;

		const std::shared_ptr<Process> findProcess(std::string processName) const;
		
		void labelProcess(std::string label, std::string processName);
		
		void labelProcess(std::string label, std::shared_ptr<Process> process);

		void addProcessLabel(std::string label, std::vector<std::shared_ptr<Process>> processes);

	private: 
		std::string name;
		std::vector<std::shared_ptr<Process>> processes;
		std::unordered_map<std::string, std::vector<std::shared_ptr<Process>>> map;
};


#endif