#include "CMSAnalysis/Filters/interface/DuplicateNumsFilter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

DuplicateNumsFilter::DuplicateNumsFilter():
{}

std::string DuplicateNumsFilter::getFilterString() const
{
    auto IDNum = inputMod->getEventIDNum();
    std::unordered_set<long> eventIDs;
    if (eventIDs.find(IDNum) != eventIDs.end()) {
        
        eventIDs.insert(IDNum);
        return "unique event ID";
    }
    else
    {
        return "";
    }
    };