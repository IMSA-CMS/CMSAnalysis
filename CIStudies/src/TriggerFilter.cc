#include "CIAnalysis/CIStudies/interface/TriggerFilter.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

TriggerFilter::TriggerFilter(const std::shared_ptr<Trigger> iTrigger) :
  trigger(iTrigger)
{
}

std::string TriggerFilter::makeFilterString(const edm::EventBase& event)
{
  std::cout << trigger->checkEvent() << " : " << trigger->checkEvent() ? trigger->getName() : "";
  return trigger->checkEvent() ? trigger->getName() : "";
}