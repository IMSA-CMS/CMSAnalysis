#include "CMSAnalysis/Filters/interface/TriggerCut.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

TriggerCut::TriggerCut(std::vector<std::string> itrigger, std::string chosenDataset) :
	triggers(itrigger)
	
{
	this->chosenDataset=chosenDataset;
	this->channelTriggerDict={ 
		{"SingleElectron", triggers[0] },
    	{ "SingleMuon", triggers[1] }
	};
	this->wrongChannelTriggerDict={
        { "SingleElectron", triggers[1]},
        { "SingleMuon", triggers[0]}
    };
	//Add the rest of trigger indices after converting map for 3+ triggers
	
}


 
bool TriggerCut::checkEventInternal(const Event& event, const EventInput* input) const
{
	auto params = input->getFileParams()->getParameters();
	auto iterator = params.find("Trigger");
	if (iterator == params.end())
	{
		for (auto trigger : triggers) 
		{
			if (input->checkTrigger(trigger)) 
			{
				//std::cout << "accepted" << std::endl;
				return true;
			}
		}
		//std::cout << "rejected" << std::endl;
		return false;
	}
	auto dataset = iterator->second;

	if (dataset == "SingleElectron")
	{
		for (auto trigger : triggers) 
		{	
			// Replace equals with contains if checking multiple valid triggers 
			if (trigger == channelTriggerDict.find(chosenDataset)->second)
			{
				if (input->checkTrigger(trigger)) 
				{
					//std::cout << "accepted" << std::endl;
					return true;
				}
			}
			// Account for double-counted triggers 
			if (trigger == wrongChannelTriggerDict.find(chosenDataset)->second)
			{
				if (input->checkTrigger(trigger)) 
				{
					//std::cout << "rejected" << std::endl;
					return false;
				}
			}
			else
			{
				//std::cout << "rejected" << std::endl;
				return false; 
			}
		}
	}
	// Single Muon (Assuming SingleElectron is the default dataset, not using mixed channels)
	else
	{
		for (auto trigger : triggers) 
		{
			if (input->checkTrigger(trigger)) 
			{
				//std::cout << "accepted" << std::endl;
				return true;
			}
		}
	}
	
	std::cout << "rejected" << std::endl;
	return false;
}


