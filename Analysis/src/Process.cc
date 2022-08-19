#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

TH1* Process::getHist(HistVariable histType, bool scaleToExpected) const
{
	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	int singleProcessNumber = 0;
	for (const auto& singleProcess : processes)
	{
		singleProcessNumber++;
		if (singleProcess.getHist(histType, false) == 0) {
			std::cout << this->name << std::endl;
			throw std::runtime_error("Histogram not found in process: " + this->name + "\nIn singleProcess number: " + singleProcessNumber);
		}
		if (singleProcess.getHist(histType, false)->GetMaximumBin() > maxBinNum)
		{
			maxBinNum = singleProcess.getHist(histType, false)->GetMaximumBin();
		}
		if (singleProcess.getHist(histType, false)->GetBarWidth() > maxBarWidth)
		{
			maxBarWidth = singleProcess.getHist(histType, false)->GetBarWidth();
		}
	}
	TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	for (const auto& singleProcess : processes)	
	{
		hist->Add(singleProcess.getHist(histType, scaleToExpected));
	}
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	//If you want yield to print while running SuperPlot uncomment the print statement (only prints the yield for the first MassTarget in the process)
	//std::cout << "Total yield for mass target " << processes.at(0).getMassTarget() << " is " << getYield(processes.at(0).getMassTarget()) << std::endl;
	return hist;
}

TH2* Process::get2DHist(HistVariable histType) const
{
	int maxBinNum = 0;
	int yMaxBinNum = 0;
	double maxBarWidth = 0.0;
	int singleProcessNumber = 0;
	for (const auto& singleProcess : processes)
	{
		singleProcessNumber++;
		if (singleProcess.get2DHist(histType) == 0) {
			std::cout << this->name << std::endl;
			throw std::runtime_error("Histogram not found in process: " + this->name + "\nIn singleProcess number: " + singleProcessNumber);
		}
		if (singleProcess.get2DHist(histType)->GetMaximumBin() > maxBinNum)
		{
			maxBinNum = singleProcess.get2DHist(histType)->GetMaximumBin();
		}
		if (singleProcess.get2DHist(histType)->GetBarWidth() > maxBarWidth)
		{
			maxBarWidth = singleProcess.get2DHist(histType)->GetBarWidth();
		}
		if (singleProcess.get2DHist(histType)->GetNbinsY() > yMaxBinNum)
		{
			yMaxBinNum = singleProcess.get2DHist(histType)->GetNbinsY();
		}
	}
	TH2* hist = new TH2F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth, yMaxBinNum, 0, yMaxBinNum * maxBarWidth);
	for (const auto& singleProcess : processes)	
	{
		hist->Add(dynamic_cast<TH2 *>(singleProcess.get2DHist(histType)));
	}
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	//If you want yield to print while running SuperPlot uncomment the print statement (only prints the yield for the first MassTarget in the process)
	//std::cout << "Total yield for mass target " << processes.at(0).getMassTarget() << " is " << getYield(processes.at(0).getMassTarget()) << std::endl;
	return hist;
}


double Process::getYield(HistVariable dataType) const
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