#include "CMSAnalysis/DataCollection/interface/HistogramPrototype.hh"
#include "CMSAnalysis/DataCollection/interface/FilterModule.hh"
#include "TH1.h"

#include <string>

bool HistogramPrototype::shouldDraw() const
{
  // std::cout << " Made it to Histogram Prototype\n";
  for (std::shared_ptr<FilterModule> filterMod : filters)
  {
    filterMod->process(); // Calls process and makes the filter string
    
    if (filterMod->getFilterString() == "")
    {
      std::cout << "empty filter string\n";
      std::cout << " Getting filter string\n";
      std::cout << filterMod->getFilterString() << "\n";
      return false; // Return false if any filterString is empty
    }
  }

  return true;
}

void HistogramPrototype::setInput(const InputModule* iInput)
{
  input = iInput;
  for(auto filter:filters)
  {
    filter->setInput(iInput);
  }
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
