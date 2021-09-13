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

  ParticleCollection getPosParticles() const;  // ParticleCollection of just the positively charged particles
  ParticleCollection getNegParticles() const;  // ParticleCollection of just the negatively charged particles
  double getNumPosParticles() const {return getPosParticles().getNumParticles();}
  double getNumNegParticles() const {return getNegParticles().getNumParticles();}

  double getInvariantMass() const;
  double getLeadingTransverseMomentum() const;
  double getNthHighestPt(int n) const;
  double getLeadingPt() const;
  double getCollinsSoper() const; 
  bool isBB() const;
  bool isBE() const;
  
  double calculateAllLeptonInvariantMass() const;
  double calculateSameSignInvariantMass(bool usingPhi = false) const;
  std::vector<double> calculateSameSignInvariantMasses(bool usingPhi = false) const;
  double calculateOppositeSignInvariantMass() const;

  double calculateRecoveredInvariantMass(int nLeptons, int motherPDGID) const;

  Particle getLeadingPtLepton() const;
  int getLeptonTypeCount(Particle::Type leptonType) const;

  void clear() {particles.clear();}

private:
  std::vector<Particle> particles;

  std::pair<Particle, Particle> chooseParticles() const; // picks particles with greatest invariant mass
  std::pair<Particle, Particle> chooseParticles(bool oppositeSigns) const; // picks particles given if they are opposite signs or not
  std::pair<Particle, Particle> chooseParticlesByPhi(bool oppositeSigns) const; // picks particles by the Phi angle

  bool checkSigns(Particle particle1, Particle particle2) const;

  double calculateInvariantMass(Particle particle1, Particle particle2) const;

  double calculateLeadingTransverseMomentum(Particle particle1, Particle particle2) const;
  double calculateCollinsSoper(Particle particle1, Particle particle2) const;


  double calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar) const;
  bool lowEtaFlip(Particle particle, Particle antiparticle) const;
};

#endif
