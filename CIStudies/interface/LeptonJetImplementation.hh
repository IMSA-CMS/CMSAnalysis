#ifndef LEPTONJETIMPLEMENTATION_HH
#define LEPTONJETIMPLEMENTATION_HH

#include <vector>
#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "CIAnalysis/CIStudies/interface/ParticleImplementation.hh"
class LeptonJet;

class LeptonJetImplementation : public ParticleImplementation

{
  public:
  virtual ~LeptonJetImplementation(){}
  LeptonJetImplementation(std::shared_ptr<LeptonJet>);
  virtual reco::Candidate::LorentzVector getFourVector() const override;
  virtual bool operator == (const ParticleImplementation& userParticle) const override;
  virtual int charge() const override {throw std::__throw_runtime_error;}
  virtual double et() const override {throw std::__throw_runtime_error;}
  virtual double energy() const override;
  virtual double isolation() const override {throw std::runtime_error("Isolation not implemented.");}
  // double getPt() const override {return getFourVector().Pt();}
  // double getPhi() const override {return getFourVector().Phi();}
  // double getEta() const override {return getFourVector().Eta();}
  // double getMass() const override{return getFourVector().mass();}
  virtual Particle mother() const override {throw std::__throw_runtime_error;}
  virtual Particle daughter(int i) const override {throw std::__throw_runtime_error;}
  virtual int numberOfDaughters() const override {throw std::__throw_runtime_error;}
  virtual Particle::Type getType() const override;
  // virtual bool isIsolated() const override {throw std::__throw_runtime_error;};
  virtual int pdgId() const override {throw std::__throw_runtime_error;};
  virtual int status() const override {throw std::__throw_runtime_error;};
  virtual bool isNotNull() const override {return true;}//lJet;}
  virtual bool isFinalState() const override {return true;}


  
  private:
    std::shared_ptr<LeptonJet> lJet;
    void checkIsNull() const;

};
  #endif