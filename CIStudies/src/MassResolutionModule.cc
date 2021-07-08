#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingPairCollection.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

MassResolutionModule::MassResolutionModule(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const std::shared_ptr<WeightingModule> weightingModule, const std::shared_ptr<LRWeightModule> lrWeightModule, const std::shared_ptr<MatchingModule> matchingModule, std::string bin, const int minMass, const int maxMass, const int massIntervalSize) :
  ResolutionModule(genSimModule, recoModule, weightingModule, lrWeightModule, matchingModule, bin, minMass, maxMass, massIntervalSize)
{
}

void MassResolutionModule::fillError(const MatchingPairCollection& bestPairs)
{
  if (bestPairs.getSize() >= 2)
    {
      double genSimMass = bestPairs.getGenParticles().getInvariantMass();
      double recoMass = bestPairs.getRecoParticles().getInvariantMass();

      std::string massBin = pickBin(genSimMass);
      fillHistogram("massResolutionHistBin" + massBin, {(recoMass - genSimMass) / genSimMass});
    }
}
