#ifndef LEPTONJET_HH
#define LEPTONJET_HH

#include <vector>
#include "CIAnalysis/CIStudies/interface/Particle.hh"

class LeptonJet
{
public:
  LeptonJet(const Particle particle);
  std::vector<double> getPt() const; // Consider four vectors, change to doubles (task for later)
  std::vector<double> getPhi() const;
  std::vector<double> getEta() const;

  const std::vector<Particle>& getParticles() const {return leptonJetParticles;}
  void addParticle(const Particle particle) {leptonJetParticles.push_back(particle);}
  int getNumParticles() const {return leptonJetParticles.size();}


private:
  std::vector<Particle> leptonJetParticles;

  std::vector<double> ptVector;
  std::vector<double> phiVector;
  std::vector<double> etaVector;

  void findPt() const;
  void findPhi() const;
  void findEta() const;
};

#endif

