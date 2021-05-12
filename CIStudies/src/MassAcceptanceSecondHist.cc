#include "CIAnalysis/CIStudies/interface/MassAcceptanceSecondHist.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include <vector>

MassAcceptanceSecondHist::MassAcceptanceSecondHist(const std::shared_ptr<MatchingModule> iMatchingModule, const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype(iname, iNBins, iminimum, imaximum),
  matchMod(iMatchingModule)
  
  
{
}

double MassAcceptanceSecondHist::value() const
{
  const MatchingPairCollection& pairs = matchMod->getMatchingBestPairs();
  if(pairs.getSize() >= 2) {
  return pairs.getGenParticles().getInvariantMass();
  }
  return -1;
}
