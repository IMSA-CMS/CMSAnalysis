#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"

class LeptonJet;

class ParticleImplementation;

// Interface for particle operations. Contains a ParticleImplementation.
class Particle
{
  public:
    // specifies particle type
    enum class Type{Electron, Muon, Photon, LeptonJet, Jet, DarkPhoton, Neutralino, None};

    // specifies barrel state
    enum class BarrelState{Barrel, Endcap, None};
    
    Particle(const reco::Candidate* iparticle = nullptr); 
    Particle(const Particle& particle1);
    Particle(const LeptonJet& leptonjet);
    Particle(reco::Candidate::LorentzVector vec, int charge, Particle::Type type, double relIso = -999);
    std::shared_ptr<ParticleImplementation> getParticleImplementation();
    Particle& operator = (const Particle& particle2);
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
    // bool isIsolated() const {throw std::runtime_error("error");}; //Lepton
    double getDeltaR(Particle particle) const;
    int getCharge() const;
    Particle::BarrelState getBarrelState() const;
    reco::Candidate::LorentzVector getFourVector() const;
    Particle::Type getType() const;
    // Static function to identify type of any particle based on pdgid.
    static Particle::Type identifyType(int pdgid); 


    // bool isGenSim() const;
    //bool isFinalState() const;
  protected:
    Particle(std::shared_ptr<ParticleImplementation> particle);
    std::shared_ptr<ParticleImplementation> getParticle(){return particle;}
    const std::shared_ptr<ParticleImplementation> getParticle() const {return particle;}
    void checkIsNull() const;
  private:
    std::shared_ptr<ParticleImplementation> particle;
    
};

#endif
