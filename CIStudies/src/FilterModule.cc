#include "CIAnalysis/CIStudies/interface/FilterModule.hh"

//update this to remove event parameter
bool FilterModule::process(const edm::EventBase& event)
{
  filterString = makeFilterString(event);
  return !filterString.empty();
}
