#ifndef LEPTONJET_HH
#define LEPTONJET_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetImplementation.hh"

class LeptonJet : public Particle
{
public:
  using Particle::Particle;
  LeptonJet();
  LeptonJet(const Particle& particle);
  // double getPt() const {return leptonJet->getPt();}
  // double getPhi() const {return leptonJet->getPhi();}
  // double getEta() const {return leptonJet->getEta();}
  // double getMass() const {return leptonJet->getMass();}

  bool operator==(LeptonJet userJet) const;

  const std::vector<Particle>& getParticles() const {return cast()->getParticles();}
  void addParticle(const Particle particle) {cast()->addParticle(particle);}
  int getNumParticles() const {return cast()->getNumParticles();}

  // Adds four vectors (reco::Candidate::LorentzVector)
  //reco::Candidate::LorentzVector getFourVector() const {return leptonJet->getFourVector();}

private:

  std::shared_ptr<LeptonJetImplementation> cast() const;
  //std::shared_ptr<LeptonJetImplementation> leptonJet;
};

#endif