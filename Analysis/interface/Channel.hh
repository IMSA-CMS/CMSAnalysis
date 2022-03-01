#ifndef CHANNEL_HH
#define CHANNEL_HH

#include <vector>

class Process;
class THStack;

public class Channel
{
	public:
		Channel(std::string name, std::vector<Process*> processes) : name(name), processes(processes) {}

		THStack* getStack(std::string categoryName = "");	
	
	private: 
		std::vector<Process*> processes;
		std::string name;
};


#endif