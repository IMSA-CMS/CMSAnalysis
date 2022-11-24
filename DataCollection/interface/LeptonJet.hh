#ifndef LEPTONJET_HH
#define LEPTONJET_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetImplementation.hh"

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

private:

  // This method takes the particle shared ptr and and casts it into a lepton jet implementation and checks if the pointer is null
  
  std::shared_ptr<LeptonJetImplementation> cast() const;
};

#endif