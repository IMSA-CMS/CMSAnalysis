#include "CMSAnalysis/Filters/interface/RunCut.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <algorithm>

RunCut::RunCut(std::string filename)
{
  // Read in JSON file and fill runNumbers list
}

bool RunCut::checkEventInternal(const Event& event, const EventInput* input) const;
{
  long runNum = inputMod->getRunNum();
  if (std::find(runNumbers.begin(), runNumbers.end(), runNum) != runNumbers.end())
  {
    return true;
  }
  else
  {
    return false;
  }
};
