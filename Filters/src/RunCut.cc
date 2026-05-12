#include "CMSAnalysis/Filters/interface/RunCut.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"
#include <algorithm>

RunCut::RunCut(std::vector<std::string> filenames)
{
  // Read in JSON files and fill runNumbers list
  for (const auto &filename : filenames)
  {
    auto jsonData = Utility::loadJSONFile(filename);
    for (const auto &runEntry : jsonData.getMemberNames())
    {
      long runNum = std::stol(runEntry);
      const auto &lumiRanges = jsonData[runEntry];
      for (const auto &lumiRange : lumiRanges)
      {
        int lumiStart = lumiRange[unsigned(0)].asInt();
        int lumiEnd = lumiRange[unsigned(1)].asInt();
        runAndLumiRanges[runNum].insert({lumiStart, lumiEnd});
      }
    }
  }
}

bool RunCut::checkEventInternal(const Event &event, const EventInput *input) const
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
    for (const auto &lumiRange : runNumbers->second)
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
