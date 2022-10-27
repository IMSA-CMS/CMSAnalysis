#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"

class ParticleType;

class LeptonJet;

class ParticleImplementation;

// Interface for particle operations. Contains a ParticleImplementation.
class Particle
{
  public: 
    
    enum class SelectionFit{Tight, Medium, Loose, None};

    // specifies barrel state
    enum class BarrelState{Barrel, Endcap, None};
    
    Particle(const reco::Candidate* iparticle = nullptr); 
    Particle(const Particle& particle1);
    Particle(const LeptonJet& leptonjet);
    Particle(reco::Candidate::LorentzVector vec, int charge, const ParticleType& type, double relIso = -999, Particle::SelectionFit fit = Particle::SelectionFit::Tight);
    Particle(reco::Candidate::LorentzVector vec, int charge, const ParticleType& type, int pid, int status, int m1, int m2,int d1, int d2, double relIso);
    std::shared_ptr<ParticleImplementation> getParticleImplementation();
    Particle& operator = (const Particle& particle2);
    std::string getName() const;
    double getPt() const;
    double getPhi() const;
    double getEta() const;
    double getEt() const;
    double getEnergy() const;
    double getMass() const;
    // double getIsolation() const;//Lepton
    bool isNotNull() const;
    bool operator == (const Particle& p1) const;
    bool operator != (const Particle& p1) const {return !(*this == p1);}
    bool operator < (const Particle& p1) const {return (getPt() < p1.getPt());}
    bool operator > (const Particle& p1) const {return (getPt() > p1.getPt());}
    // bool isIsolated() const {throw std::runtime_error("error");}; //Lepton
    double getDeltaR(Particle particle) const;
    int getCharge() const;
    Particle::BarrelState getBarrelState() const;
    reco::Candidate::LorentzVector getFourVector() const;
    const ParticleType& getType() const;
    // Static function to identify type of any particle based on pdgid.
    static const ParticleType& identifyType(int pdgid); 

  // bool isGenSim() const;
  // bool isFinalState() const;
protected:
  Particle(std::shared_ptr<ParticleImplementation> particle);
  std::shared_ptr<ParticleImplementation> getParticle() { return particle; }
  const std::shared_ptr<ParticleImplementation> getParticle() const { return particle; }
  void checkIsNull() const;

private:
  std::shared_ptr<ParticleImplementation> particle;
};

#endif
