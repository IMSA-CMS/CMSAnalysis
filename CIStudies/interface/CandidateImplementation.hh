#ifndef CANDIDATEIMPLEMENTATION_HH
#define CANDIDATEIMPLEMENTATION_HH

#include <vector>
#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "CIAnalysis/CIStudies/interface/ParticleImplementation.hh"

class CandidateImplementation : public ParticleImplementation
{
public:
  virtual ~CandidateImplementation(){}
  CandidateImplementation(const reco::Candidate*);
  virtual reco::Candidate::LorentzVector getFourVector() const override;
  // virtual double eta() const override;
  virtual bool operator == (const ParticleImplementation& p1) const override;
  virtual int charge() const override;
  virtual double et() const override;
  virtual int pdgId() const override;
  virtual int status() const override;
  virtual double energy() const override;
  virtual Particle mother() const override;
  virtual Particle daughter(int i) const override;
  virtual int numberOfDaughters() const override;
  virtual bool isNotNull() const override {return particle;}
  virtual Particle::Type getType() const override;
  // virtual bool isIsolated() const override;
  const reco::Candidate* getUnderlyingParticle() const {return particle;}
  bool isFinalState() const override;

private:
  const reco::Candidate* particle;
  void checkIsNull() const;
};

#endif