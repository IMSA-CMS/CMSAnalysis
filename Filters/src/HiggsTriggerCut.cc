#include "CMSAnalysis/Filters/interface/HiggsTriggerCut.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

 
bool HiggsTriggerCut::checkEventInternal(const Event& event, const EventInput* input) const
{
	std::vector<std::string> triggers;
	auto params = input->getFileParams()->getParameters();
	triggers.push_back("HLT_IsoMu24");
	auto iterator2 = params.find("Year");

	if(iterator2 != params.end())
	{
		if(iterator2 -> second == "2018")
		{
			triggers.push_back("HLT_Ele32_WPTight_Gsf");
		}
		else if(iterator2 -> second == "2017")
		{
			triggers.push_back("HLT_Ele35_WPTight_Gsf");
		}
		else
		{
			triggers.push_back("HLT_Ele27_WPTight_Gsf");
			triggers.push_back("HLT_Photon175");
		}
	}
	auto iterator = params.find("Trigger");
	if (iterator == params.end())
	{
		for (auto trigger : triggers) 
		{
			if (input->checkTrigger(trigger)) 
			{
				return true;
			}
		}
		return false;
	}

	auto dataset = iterator->second;
	if (dataset == "SingleElectron")
	{
		// Replace equals with contains if checking multiple valid triggers 
		if (input->checkTrigger(triggers[0])) 
		{
			return false;
		}
		
		// Account for double-counted triggers 
		else if (input->checkTrigger(triggers[1])) 
		{
			return true;
		}
		else if (triggers.size() > 2 && input->checkTrigger(triggers[2]))
		{
			return true;
		}
	}
	// Single Muon (Assuming SingleElectron is the default dataset, not using mixed channels)
	else
	{
		for (auto trigger : triggers) 
		{
			if (input->checkTrigger(trigger)) 
			{
				return true;
			}
		}
	}
	return false;
}


