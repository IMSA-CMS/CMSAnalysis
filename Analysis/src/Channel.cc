#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "THStack.h"
#include <iostream>
#include <algorithm>
#include <memory>

Channel::Channel(std::string name, std::vector<std::shared_ptr<Process>> iProcesses) : name(name)
{
	for (auto process : iProcesses)
	{
		processes.push_back(process);
	}
}

const std::shared_ptr<Process> Channel::findProcess(std::string processName) const
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

void Channel::labelProcess(std::string label, std::shared_ptr<Process> process)
{
	map[label].push_back(process);
}

void Channel::addProcessLabel(std::string label, std::vector<std::shared_ptr<Process>> processes)
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

THStack* Channel::getStack(std::string label, bool scaleToExpected) const
{
	THStack* superPlot = new THStack(name.c_str(), name.c_str());
	if (label == "")
	{
		for (auto process : processes)
		{
			std::cout << "New process" << std::endl;			
			superPlot->Add(process->getHist(scaleToExpected));
		}
	}
	else
	{
		for (const auto& process : map.find(label)->second)
		{
			superPlot->Add(process->getHist(scaleToExpected));
		}
	}
	return superPlot;
}