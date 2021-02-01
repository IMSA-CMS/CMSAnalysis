#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"
#include "CIAnalysis/CIStudies/interface/FilterModule.hh"

#include <string>

bool HistogramPrototype::shouldDraw(const edm::EventBase& event) const
{
  for (std::shared_ptr<FilterModule> filterMod : filters)
  {
    filterMod->process(event); // Calls process and makes the filter string
    
    if (filterMod->getFilterString() == "")
    {
      return false; // Return false if any filterString is empty
    }
  }

  return true;
}

std::string HistogramPrototype::getFilterString() const
{
  std::string filterStr = ""; // Start with an empty string; we will append stuff to this string later

  for (std::shared_ptr<FilterModule> filterMod : filters)
  {
    filterStr.append(filterMod->getFilterString());
  }

  return filterStr;
}
