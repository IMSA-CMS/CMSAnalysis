#ifndef LEPTONJET_HH
#define LEPTONJET_HH

#include <vector>
#include "CIAnalysis/CIStudies/interface/Particle.hh"

class LeptonJet : public Particle
{
public:
  LeptonJet(const Particle particle);
  double getPt() const {return getFourVector().Pt();}
  double getPhi() const {return getFourVector().Phi();}
  double getEta() const {return getFourVector().Eta();}
  double getMass() const {return getFourVector().mass();}

  bool operator==(LeptonJet userJet) const;

  const std::vector<Particle>& getParticles() const {return leptonJetParticles;}
  void addParticle(const Particle particle) {leptonJetParticles.push_back(particle);}
  int getNumParticles() const {return leptonJetParticles.size();}

  // Adds four vectors (reco::Candidate::LorentzVector)
  reco::Candidate::LorentzVector getFourVector() const;

private:
  std::vector<Particle> leptonJetParticles;
};

#endif