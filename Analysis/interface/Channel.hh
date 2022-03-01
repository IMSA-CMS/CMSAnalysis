#ifndef CHANNEL_HH
#define CHANNEL_HH

#include <vector>

class Process;
class THStack;

public class Channel
{
	public:
		Channel(std::string name, std::vector<Process*> processes) : name(name), processes(processes) {}

		THStack* getStack(std::string label = "");

		void labelProcess(std::string label, Process* process) {map[label].push_back(process);}

		void addProcessLabel(std::string label, std::vector<Process*> processes) {map[label] = processes;}

	private: 
		std::vector<Process*> processes;
		std::unordered_map<Process*> map;
		std::string name;
};


#endif