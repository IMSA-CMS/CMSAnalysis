#include "CMSAnalysis/Utility/interface/HistogramPrototype.hh"
#include "CMSAnalysis/Modules/interface/FilterModule.hh"
#include "TH1.h"

#include <string>

bool HistogramPrototype::shouldDraw() const
{
  for (auto filterMod : filters)
  {
    if (filterMod->getFilterString(getInput()) == "")
    {
      return false; // Return false if any filterString is empty
    }
  }

  return true;
}

void HistogramPrototype::setInput(const EventInput* iInput)
{
  input = iInput;
}

std::string HistogramPrototype::getFilterString() const
{
  std::string filterStr = ""; // Start with an empty string; we will append stuff to this string later

  for (auto filterMod : filters)
  {
    filterStr.append(filterMod->getFilterString(getInput()));
  }

  return filterStr;
}

double HistogramPrototype::eventWeight() const
{
  //std::cout << "Weight event input: " << getInput() <<std::endl;
  double weight = 1.0;
  for (auto scaleFactor : scaleFactors)
  {
    weight *= scaleFactor->getScaleFactor(getInput());
    //std::cout << "Scale Factor: " << scaleFactor->getScaleFactor(getInput()) << std::endl;
  }
  return weight;
}
