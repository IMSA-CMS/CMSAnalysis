#ifndef PARTICLEIMPLEMENTATION_HH
#define PARTICLEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Utility/interface/ParticleImplementation.hh" 


class ParticleImplementation 
{
public:
  virtual ~ParticleImplementation(){}
  // enum class Type{Electron, Muon, Photon, None};
  // enum class BarrelState{Barrel, Endcap, None};
  // explicit Particle(const reco::Candidate* iparticle);
  virtual int charge() const = 0;
  // virtual double eta() const = 0;
  virtual reco::Candidate::LorentzVector getFourVector() const = 0;

  virtual int pdgId() const = 0;
  virtual int status() const = 0;
  virtual bool doesHaveMother() const = 0;
  virtual Particle mother() const = 0;
  virtual Particle daughter(int i) const = 0;
  virtual int numberOfDaughters() const = 0;
  virtual const ParticleType& getType() const = 0;
  virtual Particle::SelectionFit getSelectionFit() const = 0;
  virtual double getDXY() const;
  virtual double getDZ() const;
  virtual bool isNotNull() const = 0;
  // virtual const reco::Candidate* getUnderlyingParticle() const = 0;
  virtual bool isFinalState() const = 0;
  double getInfo(std::string) const;
  void addInfo(std::string, double);

  virtual bool operator==(const ParticleImplementation& userJet) const =0;

 private:
  std::unordered_map<std::string, double> infoMap;

};

#endif