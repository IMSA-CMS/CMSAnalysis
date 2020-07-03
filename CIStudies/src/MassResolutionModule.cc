#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingPairCollection.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

MassResolutionModule::MassResolutionModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const MatchingModule& matchingModule, std::string bin, const int minMass, const int maxMass, const int massIntervalSize) :
  ResolutionModule(genSimModule, recoModule, weightingModule, matchingModule, bin, minMass, maxMass, massIntervalSize)
{
}

void MassResolutionModule::fillError(const MatchingPairCollection& bestPairs)
{
  if (bestPairs.getSize() >= 2)
    {
      double genSimMass = bestPairs.getGenParticles().getInvariantMass();
      double recoMass = bestPairs.getRecoParticles().getInvariantMass();

      std::string massBin = pickBin(genSimMass);
      fillHistogram("massResolutionHistBin" + massBin, (recoMass - genSimMass) / genSimMass);
    }
}
