#include "CMSAnalysis/DataCollection/interface/PtResolutionModule.hh"
#include "CMSAnalysis/DataCollection/interface/MatchingPairCollection.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

PtResolutionModule::PtResolutionModule(const std::shared_ptr<MatchingModule> matchingModule, std::string bin, const int minPt, const int maxPt, const int pTIntervalSize) :
  ResolutionModule(matchingModule, bin, minPt, maxPt, pTIntervalSize)
{
}

void PtResolutionModule::fillError(const MatchingPairCollection& bestPairs)
{
  for (auto matchingPair : bestPairs.getPairs())
    {
      double genSimPt = matchingPair.getGenParticle().getPt();
      std::string pTBin = pickBin(genSimPt);
      fillHistogram("pTResolutionHistBin" + pTBin, {matchingPair.getPtError()});
    }
}
