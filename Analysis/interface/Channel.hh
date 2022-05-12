#ifndef CHANNEL_HH
#define CHANNEL_HH

#include <vector>
#include <string>
#include <unordered_map>

class Process;
class THStack;

class Channel
{
	public:
		Channel(std::string name, std::vector<const Process*> processes) : name(name), processes(processes) {}

		THStack* getStack(std::string label = "") const;

		Process* findProcess(std::string processName) const;
		
		void labelProcess(std::string label, std::string processName);
		
		void labelProcess(std::string label, const Process* process);

		void addProcessLabel(std::string label, std::vector<const Process*> processes);

	private: 
		std::string name;
		std::vector<const Process*> processes;
		std::unordered_map<std::string, const Process*> map;
		
};


#endif