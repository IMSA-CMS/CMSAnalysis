#ifndef LEPTONJET_HH
#define LEPTONJET_HH

#include <vector>
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/LeptonJetImplementation.hh"

// This class is responsible for storing lepton jets, to use this function, construct a lepton jet by using the empty constructor

class LeptonJet : public Particle
{
public:
  using Particle::Particle;

  //An empty constructor that intializes the pointer towards Lepton Jet Implementation
  LeptonJet();
  LeptonJet(const Particle& particle);


  bool operator==(LeptonJet userJet) const;

  const std::vector<Particle>& getParticles() const {return cast()->getParticles();}
  void addParticle(const Particle particle) {cast()->addParticle(particle);}
  int getNumParticles() const {return cast()->getNumParticles();}
  // Gets the "width" or biggest possible DeltaR from all particles
  // Returns 0 if only 1 particle
  double getDeltaR() const;

private:

  // This method takes the particle shared ptr and and casts it into a lepton jet implementation and checks if the pointer is null
  
  std::shared_ptr<LeptonJetImplementation> cast() const;
};

#endif