#include "CMSAnalysis/Filters/interface/BarrelStateFilter.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"

BarrelStateFilter::BarrelStateFilter(const std::shared_ptr<MatchingModule> matchingModule) : matchMod(matchingModule){};

std::string BarrelStateFilter::getFilterString(const InputModule* inputMod) const // const edm::EventBase& event)
{
  if (matchMod->getMatchingBestPairs().getGenParticles().isBB())
  {
    return "BB";
  }
  else if (matchMod->getMatchingBestPairs().getGenParticles().isBE())
  {
    return "BE";
  }
  else
  {
    return "";
  }
};
