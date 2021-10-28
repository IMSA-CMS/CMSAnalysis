#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"

class ParticleImplementation;

class Particle
{
  public:
  
    Particle(const reco::Candidate* iparticle = nullptr);
    Particle(const Particle& particle1);
    Particle& operator = (const Particle& particle2);
    enum class Type{Electron, Muon, Photon, LeptonJet, None};
    enum class BarrelState{Barrel, Endcap, None};
    double getPt() const;
    double getPhi() const;
    double getEta() const;
    double getEt() const;
    double energy() const;
    double getMass() const;
    Particle uniqueMother() const;
    Particle finalDaughter();
    Particle findMother(int motherPDGID);
    static Particle sharedMother(int motherPDGID, Particle particle1, Particle particle2);
    static Particle sharedMother(int motherPDGID, std::vector<Particle> particles);
    bool isNotNull() const;
    bool operator == (const Particle& p1) const;
    bool operator != (const Particle& p1) const {return !(*this == p1);}
    bool isIsolated() const {throw std::runtime_error("error");};
    int numberOfDaughters() const;
    Particle daughter(int i) const;
    // const reco::Candidate getUnderlyingParticle() const {return particle;}
  
    int pdgId() const;
    int status() const;
    int charge() const;
    Particle::BarrelState getBarrelState() const;
    reco::Candidate::LorentzVector getFourVector() const;
    Particle mother() const;
    Particle::Type getType() const;

    
    // bool isGenSim() const;
    bool isFinalState() const;
	
  private:
    std::shared_ptr<ParticleImplementation> particle;
    void checkIsNull() const;
};

#endif
