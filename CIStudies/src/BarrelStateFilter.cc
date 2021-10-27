#include "CIAnalysis/CIStudies/interface/BarrelStateFilter.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"

BarrelStateFilter::BarrelStateFilter(const std::shared_ptr<MatchingModule> matchingModule) :
  matchMod(matchingModule)
{}

std::string BarrelStateFilter::makeFilterString()  // const edm::EventBase& event)
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
}
