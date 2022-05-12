#ifndef CHANNEL_HH
#define CHANNEL_HH

#include <unordered_map>

#include <string>
#include <vector>

class Process;
class THStack;

class Channel
{
	public:
		Channel(std::string name, std::vector<const Process*> processes) : name(name), processes(processes) {}

		THStack* getStack(std::string label = "") const;

		const Process* findProcess(std::string processName) const;
		
		void labelProcess(std::string label, std::string processName);
		
		void labelProcess(std::string label, const Process* process);

		void addProcessLabel(std::string label, std::vector<const Process*> processes);

	private: 
		std::string name;
		std::vector<const Process*> processes;
<<<<<<< HEAD
		std::unordered_map<std::string, const Process*> map;
		
=======
		std::unordered_map<std::string, std::vector<const Process*>> map;
>>>>>>> ded437d466687e45b887622f5bbcba68c17918c1
};


#endif