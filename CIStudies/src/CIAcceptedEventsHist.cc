#include "CIAnalysis/CIStudies/interface/CIAcceptedEventsHist.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include <vector>

CIAcceptedEventsHist::CIAcceptedEventsHist(const std::shared_ptr<MatchingModule> iMatchingModule, const std::shared_ptr<GenSimIdentificationModule> iGenSimIdentificationModule, const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  matchMod(iMatchingModule),
  genSimIdentification(iGenSimIdentificationModule)
  
  
{
}

std::vector<double> CIAcceptedEventsHist::value() const
{
  const MatchingPairCollection& pairs = matchMod->getMatchingBestPairs();
  if(pairs.getSize() >= 2) {
  return {pairs.getGenParticles().getInvariantMass()};
  }
  return {-1};
}
