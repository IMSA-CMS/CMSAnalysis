#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "CMSAnalysis/Utility/interface/Filter.hh"

FilterModule::FilterModule(std::shared_ptr<Filter> ifilter) :
  filter(ifilter)
  {}

bool FilterModule::process()
{
  filterString = filter->getFilterString(getInput());

  if (counterMap.find(filterString) == counterMap.end())
  {
    counterMap.insert({filterString, 0});
  }
  ++counterMap[filterString];

  return !filterString.empty();
}

void FilterModule::finalize()
{
  for (const auto& entry : counterMap)
  {
    std::string name = entry.first.empty() ? "Events failed filter" : entry.first;
    writeText(std::to_string(entry.second), name);
  }
}