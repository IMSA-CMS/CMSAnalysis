<<<<<<< HEAD
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/Process.hh"
=======
#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
>>>>>>> ded437d466687e45b887622f5bbcba68c17918c1
#include "THStack.h"

#include <algorithm>

<<<<<<< HEAD
Process* Channel::findProcess(std::string processName) const
=======
const Process* Channel::findProcess(std::string processName) const
>>>>>>> ded437d466687e45b887622f5bbcba68c17918c1
{
	for (auto process : processes)
	{
		if (processName == process->getName())
		{
			return process;
		}
	}
	throw std::runtime_error("No process found with the inputted name.");
}

void Channel::labelProcess(std::string label, std::string processName)
{
	auto process = findProcess(processName);
	labelProcess(label, process);
}

void Channel::labelProcess(std::string label, const Process* process)
{
	map[label].push_back(process);
}

void Channel::addProcessLabel(std::string label, std::vector<const Process*> processes)
{
	//Variable "vec" is for convenience only
	auto& vec = map[label];
	for (auto process : processes)	
	{
		if (std::find(vec.begin(), vec.end(), process) == vec.end())
		{	
			vec.push_back(process);
		}
	}
}

THStack* Channel::getStack(std::string label) const
{
	THStack* superPlot = new THStack(name.c_str(), name.c_str());
	if (label == "")
	{
		for (auto process : processes)
		{
			superPlot->Add(process->getHist());
		}
	}
	else
	{
		for (const auto& process : map.find(label)->second)
		{
			superPlot->Add(process->getHist());
		}
	}
	return superPlot;
}