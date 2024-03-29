#include "CMSAnalysis/Filters/interface/TriggerFilter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

TriggerFilter::TriggerFilter(const std::shared_ptr<Trigger> iTrigger) :
  trigger(iTrigger)
{
}

std::string TriggerFilter::getFilterString(const EventInput* inputMod) const
{
  //std::cout << trigger->checkEvent(getInput()) << " : " << (trigger->checkEvent(getInput()) ? trigger->getName() : "") << "\n";
  return trigger->checkEvent(inputMod) ? trigger->getName() : "";
}