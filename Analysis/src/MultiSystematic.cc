#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/Channel.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/MultiSystematic.hh"
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

std::pair<TH1*, TH1*> MultiSystematic::getUncertainties(TH1* original) const 
{
	const double tolerance = 1e-8;
	auto histHigh = new TH1F("High hist", "High hist", original->GetNbinsX(), original->GetXaxis()->GetXmin(), original->GetXaxis()->GetXmax());
	auto histLow = new TH1F("Low hist", "Low hist",original->GetNbinsX(), original->GetXaxis()->GetXmin(), original->GetXaxis()->GetXmax());
	for (auto system : systematics)
	{
		double deltayHigh = 0;
		double deltayLow = 0;
		std::pair<TH1*, TH1*> margins = system->getUncertainties(original);
		int binsHigh = margins.first->GetNbinsX();
		int binsLow = margins.second->GetNbinsX();
	
		// double originalContent = original->GetBinContent(15);
    	// std::cout<<"original graph content: " << originalContent << std::endl;

		// double highContent = margins.first->GetBinContent(15);
   		// std::cout<<"\nmultisystematic high graph content: " << highContent << std::endl;
		
		// double lowContent = margins.second->GetBinContent(15);
    	// std::cout<<"multisystematic low graph content: " << lowContent << std::endl;


		for (int bin = 0; bin <= binsHigh + 1; bin++)
		{
			float y_value = original->GetBinContent(bin);
			if (y_value > tolerance)
			{
				float adjusted_y_value = margins.first->GetBinContent(bin);
				float change = (adjusted_y_value - y_value)/y_value;
				deltayHigh += change*change;
				std::cout<<"delta high: " << deltayHigh << std::endl;
			}
		}

		for (int bin = 0; bin <= binsLow + 1; bin++)
		{
			float y_value = original->GetBinContent(bin);
			if (y_value > tolerance)
			{
				float adjusted_y_value = margins.second->GetBinContent(bin);
				float change = (adjusted_y_value - y_value)/y_value;
				deltayLow += change*change;
				std::cout<<"delta low: " << deltayLow << std::endl;
			}
		}
		double totalChangeHigh = sqrt(deltayHigh);
		histHigh->Fill(totalChangeHigh);
		double totalChangeLow = sqrt(deltayLow);
		histLow->Fill(totalChangeLow);
	}
	return {histHigh, histLow};
}

// std::shared_ptr<MultiSystematic> MultiSystematic::addMultiSystematic(std::shared_ptr<MultiSystematic> newSystematic)
// {
// 	for (auto newSystem : newSystematic->systematics)
// 	{	
// 		// for (auto system : systematics)
// 		// {
// 		// 	if (system->getName() == newSystem->getName())
// 		// 	{
// 		// 		system->addSystematics();
// 		// 	}
// 		// }
// 		systematics.push_back(newSystem);
// 	}
// 	return std::make_shared<MultiSystematic>(*this);
// }

std::pair<TH1*, TH1*> MultiSystematic::combineSystematics(std::vector<std::shared_ptr<MultiSystematic>> systematics, TH1* original)
{
	std::unordered_map<std::string, std::pair<TH1*, TH1*>> histMap;
	for (auto system : systematics)
	{
		for (auto newSystem : system->systematics)
		{
			if (histMap.find(newSystem->getName()) == histMap.end())
			{
				histMap[newSystem->getName()] = newSystem->getUncertainties(original);
			}
			else
			{
				std::pair<TH1*, TH1*> hist = newSystem->getUncertainties(original);
				histMap[newSystem->getName()].first->Add(hist.first);
				histMap[newSystem->getName()].second->Add(hist.second);
			}
		}
	}
	
	std::pair <TH1*, TH1*> histPair {new TH1F("High hist", "High hist", original->GetNbinsX(), 
		original->GetXaxis()->GetXmin(), original->GetXaxis()->GetXmax()), 
	new TH1F("Low hist", "Low hist", original->GetNbinsX(), original->GetXaxis()->GetXmin(),
		original->GetXaxis()->GetXmax())};

	for (auto hist : histMap)
	{
		// Add the square of each bin
		for (int bin = 0; bin <= histPair.first->GetNbinsX() + 1; ++bin)
		{
			auto upContent = hist.second.first->GetBinContent(bin);
			// std::cout << "up content: " << upContent << std::endl;
			histPair.first->SetBinContent(bin, histPair.first->GetBinContent(bin) + upContent * upContent);
			auto downContent = hist.second.second->GetBinContent(bin);
			// std::cout << "down content: " << downContent << std::endl;
			histPair.second->SetBinContent(bin, histPair.second->GetBinContent(bin) + downContent * downContent);
		}
	}

	// Take the square root of each bin
	for (int bin = 0; bin <= histPair.first->GetNbinsX() + 1; ++bin)
	{
		histPair.first->SetBinContent(bin, std::sqrt(histPair.first->GetBinContent(bin)));
		histPair.second->SetBinContent(bin, std::sqrt(histPair.second->GetBinContent(bin)));
	}
	return histPair;
}
 