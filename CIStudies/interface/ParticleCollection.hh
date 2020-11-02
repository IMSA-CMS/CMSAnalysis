#ifndef PARTICLECOLLECTION_HH
#define PARTICLECOLLECTION_HH

#include <utility>
#include <vector>
#include "TLorentzVector.h"
#include "CIAnalysis/CIStudies/interface/Particle.hh"

namespace reco
{
  class Candidate;
}

class ParticleCollection
{
public:
  void addParticle(Particle particle) {particles.push_back(particle);}

  const std::vector<Particle>& getParticles() const {return particles;}
  double getNumParticles() const {return particles.size();}

  double getInvariantMass() const;
  double getLeadingTransverseMomentum() const;
  double getCollinsSoper() const; 
  bool isBB() const;
  bool isBE() const;
  
  double calculateAllLeptonInvariantMass() const;
  double calculateSameSignInvariantMass() const;
  double calculateOppositeSignInvariantMass() const;

  void clear() {particles.clear();}

private:
  std::vector<Particle> particles;

  std::pair<Particle, Particle> chooseParticles() const; //picks particles with greatest invariant mass
  std::pair<Particle, Particle> chooseParticles(bool oppositeSigns) const; // picks particles given if they are opposite signs or not

  bool checkSigns(Particle particle1, Particle particle2) const;

  double calculateInvariantMass(Particle particle1, Particle particle2) const;

  double calculateLeadingTransverseMomentum(Particle particle1, Particle particle2) const;
  double calculateCollinsSoper(Particle particle1, Particle particle2) const;

  double calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar) const;
  bool lowEtaFlip(Particle particle, Particle antiparticle) const;
};

#endif
