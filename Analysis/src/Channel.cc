#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "THStack.h"
#include "TStyle.h"
#include "TH1.h"
#include "TAxis.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <fstream>
#include "CMSAnalysis/Utility/interface/Utility.hh"
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
	throw std::runtime_error("No process found with the inputted name " + processName);
}

std::vector<std::string> Channel::getNamesWithLabel(std::string label)
{
	std::vector<std::string> names;
	auto processes = map.find(label);
	if (processes == map.end())
	{
		throw std::runtime_error("No processes with the label " + label);		
	}
	for (const auto& process : processes->second)
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

void Channel::makeDatacard(HistVariable histType)
{
	std::string channelName = getName();
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
	datacard<<"\n";

    for(auto process : getProcesses())
    {
        auto processName=Utility::substitute(process->getName()," ", "_");
    	datacard<< std::setw(20) << std::left << processName;
    }
	
    datacard<<"\n";
    datacard<< std::setw(20) << std::left << "rate";
            
	for (auto process : getProcesses())
    {
    	auto yield=std::to_string(process->getYield(histType));
        datacard<< std::setw(20) << std::left <<yield;
    }
	datacard<<"\n";
}


void Channel::CombineDatacard(HistVariable histType)
{
	std::string channelName = getName();
	std::string filename=channelName+".txt";
	std::ofstream datacard (filename);
	if(!datacard)
	{
		throw std::runtime_error("Unable to create file");
	}
	
	datacard << "\nimax    1 number of bins\n";
    datacard << "jmax    4 number of processes minus 1\n";
	datacard << "kmax 0 number of nuisance parameters\n";
    datacard << "----------------------------------------------------------------------------------------------------------------------------------\n";
    for(auto process : getProcesses())
    {
        auto processName=Utility::substitute(process->getName()," ", "_");
    	datacard<< "shapes" << std::setw(20)<< processName << std::setw(20) << channelName << std::setw(40) << "Genhiggsworkspace.root  " << "higgsworkspace.root:" << processName << "\n"; 
    }
	datacard<< "shapes				data_obs" << std::setw(20) <<channelName << std::setw(50) <<"Genhiggsworkspace.root Genhiggsworkspace:" << channelName << "Events\n";

	datacard << "----------------------------------------------------------------------------------------------------------------------------------\n";
	datacard <<"bin    ";
	
	for (auto i = 0; i < 5; i++) 
	{
  		 datacard <<std::setw(20) << std::left << channelName;
	}
   
	datacard<<"\n";

    datacard<< std::setw(20) << std::left << "process"<< std::left;

    for(auto process : getProcesses())
    {
        auto processName=Utility::substitute(process->getName()," ", "_");
    	datacard<< std::setw(20) << std::left << processName;
    }
	datacard<<"\n";
	
	datacard<< std::setw(20) << std::left << "process"<< std::left;

	for (auto i = 0; i < 5; i++) 
	{
		if(i <4)
  		datacard <<std::setw(20) << std::left << i;
		else
		datacard <<std::setw(20) << std::left << -1;
	}
   


    datacard<<"\n";
    datacard<< std::setw(20) << std::left << "rate";
            
	for (auto process : getProcesses())
    {
    	auto yield=std::to_string(process->getYield(histType));
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

THStack* Channel::getStack(HistVariable histType, std::string label, bool scaleToExpected, int rebinConstant) const
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

	if (superPlot->GetHists() == nullptr) {
    	delete superPlot;  
    	return nullptr;
	}

	return superPlot;
}

std::vector<TH1*> Channel::getHists(HistVariable histType, std::string label, bool scaleToExpected) const
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