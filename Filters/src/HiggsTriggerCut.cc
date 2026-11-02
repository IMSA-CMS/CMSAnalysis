#include "CMSAnalysis/Filters/interface/HiggsTriggerCut.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

 
bool HiggsTriggerCut::checkEventInternal(const Event& event, const EventInput* input) const
{
	std::vector<std::string> muonTriggers;
	std::vector<std::string> electronTriggers;
	auto params = input->getFileParams()->getParameters();
	//triggers.push_back("HLT_IsoMu24");

	auto yearIterator = params.find("Year");
	if (yearIterator == params.end())
	{
		muonTriggers = {"HLT_IsoMu24"};
	}
	else if (yearIterator->second.find("2018") != std::string::npos)
	{
		muonTriggers = {"HLT_IsoMu24"};
		electronTriggers = {"HLT_Ele32_WPTight_Gsf"};
	}
	else if (yearIterator->second.find("2017") != std::string::npos)
	{
		muonTriggers = {"HLT_IsoMu27"};
		electronTriggers = {"HLT_Ele35_WPTight_Gsf"};
	}
	else
	{
		// 2016
		muonTriggers = {"HLT_IsoMu24", "HLT_IsoTkMu24"};
		electronTriggers = {"HLT_Ele27_WPTight_Gsf"};
	}

	bool muonFired = false;
	for (const auto& trigger : muonTriggers)
	{
		if (input->checkTrigger(trigger))
		{
			muonFired = true;
			break;
		}
	}

	bool electronFired = false;
	for (const auto& trigger : electronTriggers)
	{
		if (input->checkTrigger(trigger))
		{
			electronFired = true;
			break;
		}
	}

	auto datasetIterator = params.find("Trigger");

	// Monte Carlo: no dataset label, accept any trigger
	if (datasetIterator == params.end())
	{
		return muonFired || electronFired;
	}

	// SingleElectron stream: electron only events, muon events belong to the muon stream
	if (datasetIterator->second == "SingleElectron")
	{
		return electronFired && !muonFired;
	}

	// Muon stream: every event with a muon trigger, with or without an electron trigger
	return muonFired;
}