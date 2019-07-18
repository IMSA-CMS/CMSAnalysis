#include "CIAnalysis/CIStudies/interface/MatchingPairCollection.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

ParticleCollection MatchingPairCollection::getGenParticles() const
{
  ParticleCollection genParticles;

  for (auto& matchingPair : matchingPairCollection)
    {
      genParticles.addParticle(matchingPair.getGenParticle());
    }

  return genParticles;
}

ParticleCollection MatchingPairCollection::getRecoParticles() const
{
  ParticleCollection recoParticles;

  for (auto& matchingPair : matchingPairCollection)
    {
      recoParticles.addParticle(matchingPair.getRecoParticle());
    }

  return recoParticles;
}
