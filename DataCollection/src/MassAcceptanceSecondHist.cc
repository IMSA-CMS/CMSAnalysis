#include "CMSAnalysis/DataCollection/interface/MassAcceptanceSecondHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingModule.hh"
#include <vector>

MassAcceptanceSecondHist::MassAcceptanceSecondHist(const std::shared_ptr<MatchingModule> iMatchingModule, const std::string iname, int iNBins, double iminimum, double imaximum) : HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
                                                                                                                                                                                   matchMod(iMatchingModule)

                                                                                                                                                                                       {};

std::vector<double> MassAcceptanceSecondHist::value() const
{
  const MatchingPairCollection &pairs = matchMod->getMatchingBestPairs();
  if (pairs.getSize() >= 2)
  {
    return {pairs.getGenParticles().getInvariantMass()};
  }
  return {-1};
};
