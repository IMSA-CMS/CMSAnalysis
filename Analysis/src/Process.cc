#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "TH1F.h"

TH1* Process::getHist() const
{
	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	for (const auto& singleProcess : processes)
	{
		if (singleProcess.getHist()->GetMaximumBin() > maxBinNum)
		{
			maxBinNum = singleProcess.getHist()->GetMaximumBin();
		}
		if (singleProcess.getHist()->GetBarWidth() > maxBarWidth)
		{
			maxBarWidth = singleProcess.getHist()->GetBarWidth();
		}
	}
	TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	for (const auto& singleProcess : processes)
	{
		hist->Add(singleProcess.getHist());
	}
	hist->SetFillColor(color);
	return hist;
}