#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsHist.hh"

NLeptonsHist::NLeptonsHist(const MatchingModule& imatchModule, const std::string& iname, int iNBins, double iminimum, double imaximum):
  HistogramPrototype(iname, iNBins, iminimum, imaximum),
  matchModule(imatchModule)
{
}

double NLeptonsHist::value() const
{
  const MatchingPairCollection& pairs = matchModule.getMatchingBestPairs();
  //std::cout << pairs.getSize() << "\n";
  return pairs.getSize();
}
