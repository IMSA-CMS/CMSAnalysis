#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "THStack.h"
#include "TStyle.h"
#include "TH1.h"
#include "TAxis.h"
#include "TStyle.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream>
#include "CMSAnalysis/DataCollection/interface/Utility.hh"
#include "CMSAnalysis/Analysis/interface/Systematic.hh"
//Channel::Channel(std::string name, std::string iYAxisName, std::vector<std::shared_ptr<Process>> iProcesses) : name(name), yAxisName(iYAxisName)
Channel::Channel(std::string name, std::vector<std::shared_ptr<Process>> iProcesses) : name(name)
{
	for (auto process : iProcesses)
	{
		processes.push_back(process);
	}
}    


void addGlobalSystematic(Systematic& systematic)
{

 
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
	if(names.size() == 0) {
		throw std::runtime_error("No processes with the given label.");		
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

void Channel::makeDatacard(std::shared_ptr<Channel> channel)
{
	std::string channelName = channel->getName();
	std::string filename=channelName+".txt";
	std::ofstream datacard (filename);
	if(!datacard)
	{
		throw std::runtime_error("Unable to create file");
	}

	datacard << "\nimax    1 number of bins\n";
    datacard << "jmax    _______ number of processes minus 1\n";
    datacard << "----------------------------------------------------------------------------------------------------------------------------------\n";
	datacard <<"bin";
	
	for (auto i = 0; i < 5; i++) 
	{
  		 datacard <<std::setw(20) << std::left << channelName;
	}
   

    datacard<< std::setw(20) << std::left << "process"<< std::left;

    for(auto process : channel->getProcesses())
    {
        auto processName=Utility::substitute(process->getName()," ", "_");
    	datacard<< std::setw(20) << std::left << processName;
    }
	
    datacard<<"\n";
    datacard<< std::setw(20) << std::left << "rate";
            
	for (auto process : channel->getProcesses())
    {
    	auto yield=std::to_string(process->getYield("Invariant Mass"));
        datacard<< std::setw(20) << std::left <<yield;
    }
	datacard<<"\n";
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

std::vector<double> Channel::getYields(std::string dataType) const
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

THStack* Channel::getStack(std::string histType, std::string label, bool scaleToExpected, int rebinConstant) const
{
	THStack* superPlot = new THStack(name.c_str(), name.c_str());
	if (label == "")
	{
		for (auto process : processes)
		{	
			superPlot->Add(process->getHist(histType, scaleToExpected)->Rebin(rebinConstant));
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
			else {
				hist->SetFillColor(hist->GetLineColor());
				//hist->SetLineColor(kBlack);
				//hist->SetLineWidth(1);
			}
			hist->Rebin(rebinConstant);
			superPlot->Add(hist);
		}
	}
	return superPlot;
}

std::vector<TH1*> Channel::getHists(std::string histType, std::string label, bool scaleToExpected) const
{
	std::vector<TH1*> hists;
	if (label == "")
	{
		for (auto process : processes)
		{	
			hists.push_back(process->getHist(histType, scaleToExpected));
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
			else {
				hist->SetFillColor(hist->GetLineColor());
				//hist->SetLineColor(kBlack);
				//hist->SetLineWidth(1);
			}
			hists.push_back(hist);
		}
	}
	return hists;
}