#include "CMSAnalysis/Histograms/interface/CIAcceptedEventsHist.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
#include <vector>

CIAcceptedEventsHist::CIAcceptedEventsHist(const std::shared_ptr<MatchingModule> iMatchingModule, const std::string iname, int iNBins, double iminimum, double imaximum) : HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
                                                                                                                                                                           matchMod(iMatchingModule)
                                                                                                                                                                           // genSimIdentification(iGenSimIdentificationModule)
                                                                                                                                                                           {};

std::vector<double> CIAcceptedEventsHist::value() const
{
  const MatchingPairCollection &pairs = matchMod->getMatchingBestPairs();
  if (pairs.getSize() >= 2)
  {
    return {pairs.getGenParticles().getInvariantMass()};
  }
  return {-1};
};
