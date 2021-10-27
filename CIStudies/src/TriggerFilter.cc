#include "CIAnalysis/CIStudies/interface/TriggerFilter.hh"


TriggerFilter::TriggerFilter(const std::shared_ptr<Trigger> iTrigger) :
  trigger(iTrigger)
{
}

std::string TriggerFilter::makeFilterString()
{
  std::cout << trigger->checkEvent(getInput()) << " : " << trigger->checkEvent(getInput()) ? trigger->getName() : "";
  return trigger->checkEvent(getInput()) ? trigger->getName() : "";
}