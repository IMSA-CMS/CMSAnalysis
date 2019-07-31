#include "CIAnalysis/CIStudies/interface/MassResolutionModule.hh"
#include "CIAnalysis/CIStudies/interface/MatchingPairCollection.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

MassResolutionModule::MassResolutionModule(const MatchingModule& matchingModule, const int minMass, const int maxMass, const int massIntervalSize) :
  ResolutionModule(matchingModule, "mass", minMass, maxMass, massIntervalSize)
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
