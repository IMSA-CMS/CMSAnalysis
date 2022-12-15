#ifndef PARTICLE_HH
#define PARTICLE_HH

#include <vector>
#include "DataFormats/Candidate/interface/Candidate.h"

class ParticleType;

class LeptonJet;

class ParticleImplementation;
class GenSimParticle;
// Interface for particle operations. Contains a ParticleImplementation.
class Particle
{
  public: 
  
    // Specifies selection fit
    enum class SelectionFit{Tight, Medium, Loose, None};

    // Specifies barrel state
    enum class BarrelState{Barrel, Endcap, None};

    // Constructors
    Particle(const reco::Candidate* iparticle); 
    Particle(const Particle& particle1);
    Particle(const LeptonJet* iparticle);
    
    Particle(reco::Candidate::LorentzVector vec, int charge, const ParticleType& type, Particle::SelectionFit fit = Particle::SelectionFit::Tight);
    Particle(reco::Candidate::LorentzVector vec, int charge, const ParticleType& type, int pid, int status, int m1, int m2,int d1, int d2);
    Particle(reco::Candidate::LorentzVector vec, int charge, const ParticleType& type, const Particle* motherParticle, std::vector<const GenSimParticle*> daughters, const int status);

    static Particle nullParticle();

    //you shouldn't need to use this but it's here anyway
    std::shared_ptr<ParticleImplementation> getParticleImplementation();

    // Accessors for particle properties
    std::string getName() const;
    double getPt() const;
    double getPhi() const;
    double getEta() const;
    double getEt() const;
    double getEnergy() const;
    double getMass() const;
    bool isNotNull() const;

    // Operators
    Particle& operator = (const Particle& particle2);
    bool operator == (const Particle& p1) const;
    bool operator != (const Particle& p1) const {return !(*this == p1);}

    // For purpose of sorting by pt 
    bool operator < (const Particle& p1) const {return (getPt() < p1.getPt());}
    bool operator > (const Particle& p1) const {return (getPt() > p1.getPt());}

    // More accessors for particle properties
    double getDeltaR(Particle particle) const;
    double getInfo(std::string) const;
    void addInfo(std::string, double);
    int getCharge() const;
    Particle::BarrelState getBarrelState() const;
    reco::Candidate::LorentzVector getFourVector() const;
    const ParticleType& getType() const;

    // Static function to identify type of any particle based on pdgid.
    static const ParticleType& identifyType(int pdgid); 

protected:
  Particle(std::shared_ptr<ParticleImplementation> particle);

  // Returns particle implementation connected to particle
  std::shared_ptr<ParticleImplementation> getParticle() { return particle; }
  const std::shared_ptr<ParticleImplementation> getParticle() const { return particle; }

  void checkIsNull() const;

private:
  std::shared_ptr<ParticleImplementation> particle;
};

std::ostream& operator<<(std::ostream& str, Particle part);

#endif
