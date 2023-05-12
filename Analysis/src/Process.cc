#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "TList.h"

TH1* Process::getHist(std::string histType, bool scaleToExpected) const
{
	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	int singleProcessNumber = 0;
	for (const auto& singleProcess : processes)
	{
		singleProcessNumber++;
		if (singleProcess.getHist(histType, false) == 0) {
			throw std::runtime_error("Histogram not found in process: " + this->name + "\nIn singleProcess number: " + singleProcessNumber);
		}
		if (singleProcess.getHist(histType, false)->GetNbinsX() > maxBinNum)
		{
			maxBinNum = singleProcess.getHist(histType, false)->GetNbinsX();
		}
		if ((singleProcess.getHist(histType, false)->GetXaxis()->GetBinWidth(maxBinNum)) > maxBarWidth)
		{
			maxBarWidth = (singleProcess.getHist(histType, false)->GetXaxis()->GetBinWidth(maxBinNum));
		}
	}
	TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	TH1* toAdd;
	TList* toMerge = new TList;
	for (const auto& singleProcess : processes)	
	{
		toAdd = singleProcess.getHist(histType, scaleToExpected);
		toMerge->Add(toAdd);
	}
	hist->Merge(toMerge);
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	//If you want yield to print while running SuperPlot uncomment the print statement (only prints the yield for the first MassTarget in the process)
	//std::cout << "Total yield for mass target " << processes.at(0).getMassTarget() << " is " << getYield(processes.at(0).getMassTarget()) << std::endl;
	return hist;
}

TH2* Process::get2DHist(std::string histType) const
{
	int maxBinNum = 0;
	int yMaxBinNum = 0;
	double xMaxBarWidth = 0.0;
	double yMaxBarWidth = 0.0;
	int singleProcessNumber = 0;
	//std::cout << processes.size() << std::endl;
	for (const auto& singleProcess : processes)
	{
		singleProcessNumber++;
		if (singleProcess.get2DHist(histType) == 0) {
			throw std::runtime_error("Histogram not found in process: " + this->name + "\nIn singleProcess number: " + singleProcessNumber);
		}
		if (singleProcess.get2DHist(histType)->GetNbinsX() > maxBinNum)
		{
			maxBinNum = singleProcess.get2DHist(histType)->GetNbinsX();
		}
		if ((singleProcess.get2DHist(histType)->GetXaxis()->GetBinWidth(maxBinNum)) > xMaxBarWidth)
		{
			xMaxBarWidth = (singleProcess.getHist(histType, false)->GetXaxis()->GetBinWidth(maxBinNum));
		}
		if (singleProcess.get2DHist(histType)->GetNbinsY() > yMaxBinNum)
		{
			yMaxBinNum = singleProcess.get2DHist(histType)->GetNbinsY();
		}
		if ((singleProcess.get2DHist(histType)->GetYaxis()->GetBinWidth(yMaxBinNum)) > yMaxBarWidth)
		{
			yMaxBarWidth = (singleProcess.getHist(histType, false)->GetYaxis()->GetBinWidth(yMaxBinNum));
		}
	}
	TH2* hist = new TH2F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * xMaxBarWidth, yMaxBinNum, 0, yMaxBinNum * yMaxBarWidth);
	TList* toMerge = new TList;
	for (const auto& singleProcess : processes)	
	{
		toMerge->Add(dynamic_cast<TH2 *>(singleProcess.get2DHist(histType)));
	}
	hist->Merge(toMerge);
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	//If you want yield to print while running SuperPlot uncomment the print statement (only prints the yield for the first MassTarget in the process)
	//std::cout << "Total yield for mass target " << processes.at(0).getMassTarget() << " is " << getYield(processes.at(0).getMassTarget()) << std::endl;
	return hist;
}


double Process::getYield(std::string dataType) const
{
	double totalYield = 0;
	for(const auto& singleProcess : processes)
	{
		totalYield += singleProcess.getExpectedYield(dataType);
	}
	return totalYield;
}

void Process::addProcess(SingleProcess process)
{
	if(process.checkValidity() == true) {
		processes.push_back(process);
	}
}

int Process::getNEvents() 
{
	int total = 0;
	for(auto singleProcess : processes) {
		total += singleProcess.getTotalEvents();
	}
	return total;
}
