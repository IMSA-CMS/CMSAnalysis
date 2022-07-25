#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "THStack.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

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

std::vector<double> Channel::getYields() const
{
	std::vector<double> yields;
	for(auto process : processes)
	{
		yields.push_back(process->getYield());
	}
	return yields;
}

std::vector<std::string> Channel::getNames() const
{
	std::vector<std::string> names;
	for(auto process: processes)
	{
		names.push_back(process->getName());
	}
	return names;
}

/*std::vector<std::vector<std::string>> Channel::getData() const
{
	std::vector<std::vector<std::string>> data;
	for(auto process : processes)
	{
		for(std::vector<std::string> entry : process->getData())
		{
			data.push_back(entry);
		}
	}
	return data;
}*/

THStack* Channel::getStack(std::string label, bool scaleToExpected) const
{
	THStack* superPlot = new THStack(name.c_str(), name.c_str());
	if (label == "")
	{
		for (auto process : processes)
		{	
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