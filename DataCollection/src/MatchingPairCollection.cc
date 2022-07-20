#include "CMSAnalysis/DataCollection/interface/MatchingPairCollection.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

ParticleCollection<GenSimParticle> MatchingPairCollection::getGenParticles() const
{
  ParticleCollection<GenSimParticle> genParticles;

  for (auto& matchingPair : matchingPairCollection)
    {
      genParticles.addParticle(matchingPair.getGenParticle());
    }

  return genParticles;
}

ParticleCollection<Particle> MatchingPairCollection::getRecoParticles() const
{
  ParticleCollection<Particle> recoParticles;

  for (auto& matchingPair : matchingPairCollection)
    {
      recoParticles.addParticle(matchingPair.getRecoParticle());
    }

  return recoParticles;
}
