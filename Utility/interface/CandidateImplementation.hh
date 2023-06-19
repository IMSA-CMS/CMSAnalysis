#ifndef CANDIDATEIMPLEMENTATION_HH
#define CANDIDATEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include "CMSAnalysis/Utility/interface/ParticleImplementation.hh"
#include "CMSAnalysis/Utility/interface/Muon.hh"
#include "CMSAnalysis/Utility/interface/Electron.hh"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"


class CandidateImplementation : public ParticleImplementation
{
public:
  virtual ~CandidateImplementation(){}
  CandidateImplementation(const reco::Candidate*);
  virtual reco::Candidate::LorentzVector getFourVector() const override;
  virtual bool operator == (const ParticleImplementation& p1) const override;

  //virtual double dxy() const override {throw std::runtime_error("dxy not implemented for this particle implementation");}
  //virtual double dz() const {throw std::runtime_error("dz not implemented for this particle implementation");}

  virtual int charge() const override;
  virtual int pdgId() const override;
  virtual int status() const override;
  //virtual double isolation() const override;
  virtual Particle mother() const override;
  virtual Particle daughter(int i) const override;
  virtual bool doesHaveMother() const override {return true;}
  virtual int numberOfDaughters() const override;
  virtual bool isNotNull() const override {return particle;}
  virtual const ParticleType& getType() const override;
  virtual Particle::SelectionFit getSelectionFit() const override;

  const reco::Candidate* getUnderlyingParticle() const {return particle;}
  bool isFinalState() const override;

private:
  const reco::Candidate* particle;
  void checkIsNull() const;
  const pat::Muon* getMuon() const;
  const pat::Electron* getElectron() const;
};

#endif
