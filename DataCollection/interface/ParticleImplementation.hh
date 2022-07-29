#ifndef PARTICLEIMPLEMENTATION_HH
#define PARTICLEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"

class ParticleImplementation 
{
public:
  virtual ~ParticleImplementation(){}
  // enum class Type{Electron, Muon, Photon, None};
  // enum class BarrelState{Barrel, Endcap, None};
  // explicit Particle(const reco::Candidate* iparticle);
  virtual int charge() const = 0;
  // virtual double eta() const = 0;
  virtual double isolation() const = 0;
  virtual reco::Candidate::LorentzVector getFourVector() const = 0;
  virtual int pdgId() const = 0;
  virtual int status() const = 0;
  virtual Particle mother() const = 0;
  virtual Particle daughter(int i) const = 0;
  virtual int numberOfDaughters() const = 0;
  virtual Particle::Type getType() const = 0;
  virtual bool isNotNull() const = 0;
  bool isIsolated() {throw std::runtime_error("error");};
  // virtual const reco::Candidate* getUnderlyingParticle() const = 0;
  virtual bool isFinalState() const = 0;

  virtual bool operator==(const ParticleImplementation& userJet) const =0;

 


};

#endif