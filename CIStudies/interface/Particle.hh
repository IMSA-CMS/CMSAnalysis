#ifndef PARTICLE_HH
#define PARTICLE_HH

#include "DataFormats/Candidate/interface/Candidate.h"

class Particle
{
public:
  enum class Type{Electron, Muon, Photon, None};
  enum class BarrelState{Barrel, Endcap, None};
  explicit Particle(const reco::Candidate* iparticle);
  int charge() const;
  double pt() const;
  double eta() const;
  double phi() const;
  double et() const;
  double energy() const;
  reco::Candidate::LorentzVector fourVector() const;
  int pdgId() const;
  int status() const;
  Particle mother() const;
  Particle uniqueMother() const;
  Particle daughter(int i) const;
  int numberOfDaughters() const;
  Particle finalDaughter();
  Particle findMother(int motherPDGID);
  static Particle sharedMother(int motherPDGID, Particle particle1, Particle particle2);
  static Particle sharedMother(int motherPDGID, std::vector<Particle> particles);
  bool operator==(Particle userParticle) const {return userParticle.particle == particle;}
  bool operator!=(Particle userParticle) const {return userParticle.particle != particle;}
  bool isNotNull() const {return particle;}
  Type getType() const;
  BarrelState getBarrelState() const;
  bool isIsolated() const;
  const reco::Candidate* getUnderlyingParticle() const {return particle;}
	
private:
  const reco::Candidate* particle;
  void checkIsNull() const;
};

#endif
