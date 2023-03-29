#include "CMSAnalysis/DataCollection/interface/TriggerFilter.hh"

TriggerFilter::TriggerFilter(const std::shared_ptr<Trigger> iTrigger) :
  trigger(iTrigger)
{
}

std::string TriggerFilter::makeFilterString()
{
  //std::cout << trigger->checkEvent(getInput()) << " : " << (trigger->checkEvent(getInput()) ? trigger->getName() : "") << "\n";
  return trigger->checkEvent(getInput()) ? trigger->getName() : "";
}