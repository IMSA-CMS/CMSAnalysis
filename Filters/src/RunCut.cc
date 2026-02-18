#include "CMSAnalysis/Filters/interface/RunCut.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include <algorithm>

RunCut::RunCut(std::string filename)
{
  // Read in JSON file and fill runNumbers list
}

bool RunCut::checkEventInternal(const Event& event, const EventInput* input) const
{
	if (input->getFileParams()->getProcess() != "Data")
  {
    return true; // No run cut for MC
  }

  long runNum = input->getRunNum();
  auto runNumbers = runAndLumiRanges.find(runNum);
  if (runNumbers != runAndLumiRanges.end())
  {
    long lumiBlock = input->getLumiBlock();
    for (const auto& lumiRange : runNumbers->second)
    {
      if (lumiBlock >= lumiRange.first && lumiBlock <= lumiRange.second)
      {
        return true;
      }
    }
  }
  else
  {
    return false;
  }
  return false;
}
