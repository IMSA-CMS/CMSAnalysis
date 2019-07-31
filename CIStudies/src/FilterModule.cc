#include "CIAnalysis/CIStudies/interface/FilterModule.hh"

bool FilterModule::process(const edm::EventBase& event)
{
  filterString = makeFilterString(event);
  return !filterString.empty();
}
