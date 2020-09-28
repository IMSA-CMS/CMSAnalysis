#ifndef PARTICLECOLLECTION_HH
#define PARTICLECOLLECTION_HH

#include <utility>
#include <vector>
#include "TLorentzVector.h"

namespace reco
{
  class Candidate;
}

class ParticleCollection
{
public:
  void addParticle(const reco::Candidate* particle) {particles.push_back(particle);}

  const std::vector<const reco::Candidate*>& getParticles() const {return particles;}
  double getNumParticles() const {return particles.size();}

  double getInvariantMass() const;
  double getLeadingTransverseMomentum() const;
  double getCollinsSoper() const; 

  void clear() {particles.clear();}

private:
  std::vector<const reco::Candidate*> particles;

  std::pair<const reco::Candidate*, const reco::Candidate*> chooseParticles() const; //picks particles with greatest invariant mass

  bool checkSigns(const reco::Candidate* particle1, const reco::Candidate* particle2) const;
  double calculateInvariantMass(const reco::Candidate* particle1, const reco::Candidate* particle2) const;
  double calculateLeadingTransverseMomentum(const reco::Candidate* particle1, const reco::Candidate* particle2) const;
  double calculateCollinsSoper(const reco::Candidate* particle1, const reco::Candidate* particle2) const;
  double calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar) const;
  bool lowEtaFlip(const reco::Candidate* particle, const reco::Candidate* antiparticle) const;
};

#endif
