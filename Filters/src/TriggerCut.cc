#include "CMSAnalysis/Filters/interface/TriggerCut.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

TriggerCut::TriggerCut(std::vector<std::string> itriggers) :
	triggers(itriggers)
{}
 
bool TriggerCut::checkEventInternal(const Event& event, const EventInput* input) const
{
	auto params = input->getFileParams()->getParameters();
	auto dataset = params.find("Trigger")->second;
	if (dataset == "SingleElectron")
	{
		for (auto trigger : triggers) 
		{	
			if (trigger == "HLT_IsoMu24")
			{
				if (input->checkTrigger(trigger)) 
				{
					return false;
				}
			}
			if (trigger == "HLT_Ele27_WPTight_Gsf")
			{
				if (input->checkTrigger(trigger)) 
				{
					return true;
				}
			}
			else
			{
				return false; 
			}
		}
	}
	// Single Muon
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