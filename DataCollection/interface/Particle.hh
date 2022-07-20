#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"

class LeptonJet;

class ParticleImplementation;

class Particle
{
  public:

    enum class Type{Electron, Muon, Photon, LeptonJet, Jet, DarkPhoton, Neutralino, None};
    enum class BarrelState{Barrel, Endcap, None};
    Particle(const reco::Candidate* iparticle = nullptr); 
    Particle(const Particle& particle1);
    Particle(const LeptonJet& leptonjet);
    Particle(reco::Candidate::LorentzVector vec, int charge, Particle::Type type, double relIso = -999);
    Particle(reco::Candidate::LorentzVector vec, int charge, Particle::Type type, int pid, int status, int m1, int m2,int d1, int d2, double relIso);
    std::shared_ptr<ParticleImplementation> getParticleImplementation();
    Particle& operator = (const Particle& particle2);
    double getPt() const;
    double getPhi() const;
    double getEta() const;
    double getEt() const;
    double energy() const;
    double getMass() const;
    // double getIsolation() const;//Lepton
    bool isNotNull() const;
    bool operator == (const Particle& p1) const;
    bool operator != (const Particle& p1) const {return !(*this == p1);}
    // bool isIsolated() const {throw std::runtime_error("error");}; //Lepton
    double getDeltaR(Particle particle) const;
    int charge() const;
    Particle::BarrelState getBarrelState() const;
    reco::Candidate::LorentzVector getFourVector() const;
    Particle::Type getType() const;
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
