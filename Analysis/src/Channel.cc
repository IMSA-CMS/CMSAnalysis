#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "THStack.h"
#include "TStyle.h"
#include "TH1.h"
#include "TAxis.h"
#include "TStyle.h"
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>

//Channel::Channel(std::string name, std::string iYAxisName, std::vector<std::shared_ptr<Process>> iProcesses) : name(name), yAxisName(iYAxisName)
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

std::vector<std::string> Channel::getNamesWithLabel(std::string label)
{
	std::vector<std::string> names;
	for (const auto& process : map.find(label)->second)
	{
		names.push_back(process->getName());
	}
	return names;
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

std::vector<double> Channel::getYields(HistVariable dataType) const
{
	std::vector<double> yields;
	for(auto process : processes)
	{
		yields.push_back(process->getYield(dataType));
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

THStack* Channel::getStack(HistVariable histType, std::string label, bool scaleToExpected) const
{
	THStack* superPlot = new THStack(name.c_str(), name.c_str());
	if (label == "")
	{
		for (auto process : processes)
		{	
			superPlot->Add(process->getHist(histType, scaleToExpected));
		}
	}
	else
	{
		for (const auto& process : map.find(label)->second)
		{
			TH1* hist = process->getHist(histType, scaleToExpected);
			if(label == "signal") {
				hist->SetLineColor(kRed);
				hist->SetFillColor(kWhite);
			}
			//hist->GetYaxis()->SetTitle(yAxisName.c_str());
			superPlot->Add(hist);
		}
	}
	return superPlot;
}