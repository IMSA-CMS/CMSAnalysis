#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/MultiSystematic.hh"
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
#include <fstream> //change include paths


MultiSystematic::MultiSystematic(std::string name) :
    Systematic(name)
{}


std::string MultiSystematic::getString() const
{
    throw std::runtime_error("getString not implemented");
}

void MultiSystematic::addSystematic(std::shared_ptr<Systematic> systematic)
{
	systematics.push_back(systematic);
}

std::pair<TH1*, TH1*> MultiSystematic::adjustHistogram(TH1* original) const 
{
	auto histHigh = new TH1F("High hist", "High hist", original->GetNbinsX(), original->GetXaxis()->GetXmin(), original->GetXaxis()->GetXmax());
	auto histLow = new TH1F("Low hist", "Low hist",original->GetNbinsX(), original->GetXaxis()->GetXmin(), original->GetXaxis()->GetXmax());
	for (auto system : systematics)
	{
		double deltayHigh = 0;
		double deltayLow = 0;
		std::pair<TH1*, TH1*> margins = system->adjustHistogram(original);
		int binsHigh = margins.first->GetNbinsX();
		int binsLow = margins.second->GetNbinsX();
		for(int bin = 0; bin <= binsHigh + 1; bin++)
		{
			double y_value = margins.first->GetBinContent(bin);
			double adjusted_y_value = margins.first->GetBinContent(bin);
			double change = (adjusted_y_value - y_value)/y_value;
			deltayHigh += change*change;
		}

		for(int bin = 0; bin <= binsLow + 1; bin++)
		{
			double y_value = margins.second->GetBinContent(bin);
			double adjusted_y_value = margins.second->GetBinContent(bin);
			double change = (adjusted_y_value - y_value)/y_value;
			deltayLow += change*change;
		}
		double totalChangeHigh = sqrt(deltayHigh);
		histHigh->Fill(totalChangeHigh);
		double totalChangeLow = sqrt(deltayLow);
		histLow->Fill(totalChangeLow);
	}
	return {histHigh, histLow};
}
 