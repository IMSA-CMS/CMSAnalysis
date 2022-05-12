#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "THStack.h"

#include <algorithm>

Process* Channel::findProcess(std::string processName) const
{

}

void Channel::labelProcess(std::string label, std::string processName)
{
	Process* process = findProcess(processName);
	labelProcess(label, process);
}

void Channel::labelProcess(std::string label, const Process* process)
{
	map[label].push_back(process);
}

void Channel::addProcessLabel(std::string label, std::vector<const Process*> processes)
{
	//Variable "vec" is for convenience only
	auto* vec = map[label];
	for (auto process : processes)	
	{
		if (std::find(vec->begin(), vec->end(), process) = vec->end())
		{	
			vec->push_back(process);
		}
	}
}

THStack* Channel::makeStack(std::string label)
{
	THStack* superPlot = new THStack(name.c_string(), name.c_string())
	if (categoeryName == "")
	{
		for (Process* process : processes)
		{
			superPlot->Add(process->getHist());
		}
	}
	else
	{
		for (Process* process : map[label])
		{
			superPlot->Add(process->getHist());
		}
	}
	return superPlot;
}