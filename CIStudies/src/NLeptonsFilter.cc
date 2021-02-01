#include "CIAnalysis/CIStudies/interface/NLeptonsFilter.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"

NLeptonsFilter::NLeptonsFilter(const std::shared_ptr<MatchingModule> imatchModule) :
  matchModule(imatchModule)
{
}

std::string NLeptonsFilter::makeFilterString(const edm::EventBase& event)
{
  auto pairs = matchModule->getMatchingBestPairs();
  //std::cout << pairs.getSize() << "\n";
  return std::to_string(static_cast<int>(pairs.getSize()));
}
