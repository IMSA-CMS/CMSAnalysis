#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "TH1F.h"
#include <iostream>

TH1* Process::getHist(bool scaleToExpected) const
{
	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	int singleProcessNumber = 0;
	double totalYield = 0.0;
	for (const auto& singleProcess : processes)
	{
		singleProcessNumber++;
		//std::cout << singleProcess.getName() << std::endl;
		if (singleProcess.getHist(false) != 0) {
			std::cout << "Histogram found" << std::endl;
		}
		else {
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
		totalYield += singleProcess.getExpectedYield();
	}
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	std::cout << "Total yield is " << totalYield << std::endl;
	return hist;
}
