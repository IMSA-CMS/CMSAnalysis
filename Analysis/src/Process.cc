#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "TH1F.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

TH1* Process::getHist(bool scaleToExpected) const
{
	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	int singleProcessNumber = 0;
	for (const auto& singleProcess : processes)
	{
		singleProcessNumber++;
		//std::cout << singleProcess.getName() << std::endl;
		/*if (singleProcess.getHist(false) != 0) {
			std::cout << "Histogram found" << std::endl;
		}*/
		if (singleProcess.getHist(false) == 0) {
			std::cout << this->name << std::endl;
			throw std::runtime_error("Histogram not found in process: " + this->name + "\nIn singleProcess number: " + singleProcessNumber + "\nWith histName: " + singleProcess.getInput()->getName());
		}
		if (singleProcess.getHist(false)->GetMaximumBin() > maxBinNum)
		{
			maxBinNum = singleProcess.getHist(false)->GetMaximumBin();
		}
		if (singleProcess.getHist(false)->GetBarWidth() > maxBarWidth)
		{
			maxBarWidth = singleProcess.getHist(false)->GetBarWidth();
		}
	}
	TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	for (const auto& singleProcess : processes)	
	{
		hist->Add(singleProcess.getHist(scaleToExpected));
	}
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	//If you want yield to print while running SuperPlot uncomment the print statement (only prints the yield for the first MassTarget in the process)
	//std::cout << "Total yield for mass target " << processes.at(0).getMassTarget() << " is " << getYield(processes.at(0).getMassTarget()) << std::endl;
	return hist;
}

double Process::getYield() const
{
	double totalYield = 0;
	for(const auto& singleProcess : processes)
	{
		totalYield += singleProcess.getExpectedYield();
	}
	return totalYield;
}
/*
std::vector<std::vector<std::string>> Process::getData() const
{
	std::vector<std::vector<std::string>> data;
	std::vector<double> massTargets;
	std::vector<std::string> toAdd(3, "");
	std::string massTargetString;
	std::string yieldString;
	bool doneCutting = false;
	for(const auto& singleProcess : processes) {
		//Only adds a new vector to data if it has a different massTarget. Returned vectors in data are in the format <name, massTarget, expectedYield>
		if(std::find(massTargets.begin(), massTargets.end(), singleProcess.getMassTarget()) == massTargets.end())
		{
			massTargets.push_back(singleProcess.getMassTarget());
			toAdd.at(0) = name;
			massTargetString = std::to_string(singleProcess.getMassTarget());
			doneCutting = false;
			//Removes zeroes that happen when converting double to string
			for(int index = massTargetString.length() - 1; index >= 0; index--) {
				if((massTargetString.at(index) == '0' || massTargetString.at(index) == '.') && doneCutting == false) {
					if(massTargetString.at(index) == '.') {
						doneCutting = true;
					}
					massTargetString.pop_back();
				}
				else {
					doneCutting = true;
				}
			}
			yieldString = std::to_string(getYield(singleProcess.getMassTarget()));
			doneCutting = false;
			//Same as the above for loop
			for(int index = yieldString.length() - 1; index >= 0; index--) {
				if((yieldString.at(index) == '0' || yieldString.at(index) == '.') && doneCutting == false) {
					if(yieldString.at(index) == '.') {
						doneCutting = true;
					}
					yieldString.pop_back();
				}
				else {
					doneCutting = true;
				}
			}
			toAdd.at(1) = massTargetString;
			toAdd.at(2) = yieldString;
			data.push_back(toAdd);
		}
	}
	return data;
}*/
