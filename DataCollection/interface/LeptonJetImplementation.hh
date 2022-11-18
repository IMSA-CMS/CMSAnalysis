#ifndef LEPTONJETIMPLEMENTATION_HH
#define LEPTONJETIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"
class LeptonJet;

class LeptonJetImplementation : public ParticleImplementation

{
  public:
  virtual ~LeptonJetImplementation(){}
  LeptonJetImplementation();
  LeptonJetImplementation(const LeptonJet*);
  virtual reco::Candidate::LorentzVector getFourVector() const override;
  virtual bool operator == (const ParticleImplementation& userParticle) const override;
  virtual int charge() const override {throw std::__throw_runtime_error;}
  virtual double isolation() const override {throw std::runtime_error("Isolation not implemented.");}
  double getPt() const {return getFourVector().Pt();}
  double getPhi() const {return getFourVector().Phi();}
  double getEta() const {return getFourVector().Eta();}
  double getMass() const {return getFourVector().mass();}
  virtual double dxy() const {throw std::runtime_error("getDxy not implemented");}
  virtual double dz() const {throw std::runtime_error("getDxy not implemented");}
  const std::vector<Particle>& getParticles() const {return leptonJetParticles;}
  void addParticle(const Particle particle) {leptonJetParticles.push_back(particle);}
  int getNumParticles() const {return leptonJetParticles.size();}
  virtual Particle mother() const override {throw std::__throw_runtime_error;}
  virtual Particle daughter(int i) const override {throw std::__throw_runtime_error;}
  virtual int numberOfDaughters() const override {throw std::__throw_runtime_error;}
  virtual const ParticleType& getType() const override;
  virtual Particle::SelectionFit getSelectionFit() const override {throw std::runtime_error("getSelectionFit not implemented");};
  // virtual bool isIsolated() const override {throw std::__throw_runtime_error;};
  virtual int pdgId() const override {throw std::__throw_runtime_error;};
  virtual int status() const override {throw std::__throw_runtime_error;};
  virtual bool isNotNull() const override {return true;}//lJet;}
  virtual bool isFinalState() const override {return true;}


  
  private:
    std::vector<Particle> leptonJetParticles;
    //std::shared_ptr<LeptonJet> lJet;
    void checkIsNull() const;

};
  #endif