#include "CMSAnalysis/Filters/interface/TriggerCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"

TriggerCut::TriggerCut(std::vector<std::string> itriggers) :
	triggers(itriggers)
{}

bool TriggerCut::checkEventInternal(const Event& event, const EventInput* input) const
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