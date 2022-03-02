#include "CMSAnalysis/DataCollection/interface/FilterModule.hh"

bool FilterModule::process()
{
  filterString = makeFilterString();

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