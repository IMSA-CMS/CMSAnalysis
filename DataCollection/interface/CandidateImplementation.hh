#ifndef CANDIDATEIMPLEMENTATION_HH
#define CANDIDATEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleType.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/Muon.hh"
#include "CMSAnalysis/DataCollection/interface/Electron.hh"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

class CandidateImplementation : public ParticleImplementation
{
public:
  virtual ~CandidateImplementation(){}
  CandidateImplementation(const reco::Candidate*);
  virtual reco::Candidate::LorentzVector getFourVector() const override;
  // virtual double eta() const override;
  virtual bool operator == (const ParticleImplementation& p1) const override;
  virtual int charge() const override;
  virtual int pdgId() const override;
  virtual int status() const override;
  virtual double isolation() const override;
  virtual Particle mother() const override;
  virtual Particle daughter(int i) const override;
  virtual int numberOfDaughters() const override;
  virtual bool isNotNull() const override {return particle;}
  virtual const ParticleType& getType() const override;
  virtual Particle::SelectionFit getSelectionFit() const override;
  // virtual bool isIsolated() const override;
  const reco::Candidate* getUnderlyingParticle() const {return particle;}
  bool isFinalState() const override;

private:
  const reco::Candidate* particle;
  void checkIsNull() const;
  const pat::Muon* getMuon() const;
  const pat::Electron* getElectron() const;
};

#endif
