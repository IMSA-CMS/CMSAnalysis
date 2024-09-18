#include "CMSAnalysis/Filters/interface/RunFilter.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <algorithm>


std::string RunFilter::getFilterString(const EventInput* inputMod) const
{
  long runNum = inputMod->getRunNum();
  if (std::find(runNumbers.begin(), runNumbers.end(), runNum) != runNumbers.end())
  {
    return "Pass_Run";
  }
  else
  {
    return "";
  }
};
