#include "CMSAnalysis/Filters/interface/RepeatedEventCuts.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

bool RepeatedEventCuts::checkEventInternal(const Event& event, const EventInput* input) const
{
    const auto eventID = input->getEventIDNum();

    if (eventIDs.find(eventID) != eventIDs.end())
    {
        return false;
    }
    
    eventIDs.insert(eventID);
    
    return true;
    }