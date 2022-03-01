#include "Analysis/interface/Process.hh"
#include "TH1.h"

TH1* Process::getHist()
{
	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	for (Process* singleProcess : processes)
	{
		if (singleProcess->getHist()->GetMaximumBin() > maxBinNum)
		{
			maxGBin = singleProcess->getHist()->GetMaximumBin();
		}
		if (singleProcess->getHist()->GetBarWidth() > maxBarWidth)
		{
			maxBarWidth = singleProcess->getHist()->GetBarWidth();
		}
	}
	TH1* hist = new TH1(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth)
	for (Process* singleProcess : processes)
	{
		hist->Add(*(singleProcess->getHist()));
	}
	hist-SetFillColor(color);
	return hist;
}