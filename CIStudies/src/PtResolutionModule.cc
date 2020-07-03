#include "CIAnalysis/CIStudies/interface/PtResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingPairCollection.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

PtResolutionModule::PtResolutionModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const MatchingModule& matchingModule, std::string bin, const int minPt, const int maxPt, const int pTIntervalSize) :
  ResolutionModule(genSimModule, recoModule, weightingModule, matchingModule, bin, minPt, maxPt, pTIntervalSize)
{
}

void PtResolutionModule::fillError(const MatchingPairCollection& bestPairs)
{
  for (auto matchingPair : bestPairs.getPairs())
    {
      double genSimPt = matchingPair.getGenParticle()->pt();
      std::string pTBin = pickBin(genSimPt);
      fillHistogram("pTResolutionHistBin" + pTBin, matchingPair.getPtError());
    }
}
