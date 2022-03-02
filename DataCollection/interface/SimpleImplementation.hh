#ifndef SIMPLEIMPLEMENTATION_HH
#define SIMPLEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"

class SimpleImplementation : public ParticleImplementation
{
    public:
        virtual ~SimpleImplementation(){}
        SimpleImplementation(reco::Candidate::LorentzVector vec, int ch, Particle::Type type, double iIsolation = -999);
        virtual reco::Candidate::LorentzVector getFourVector() const override;
        virtual int charge() const override;
        virtual double et() const override {throw std::runtime_error("et() error");}
        virtual double energy() const override{throw std::runtime_error("energy() error");}
        virtual double isolation() const override {return particleIsolation;}
        virtual int pdgId() const override{throw std::runtime_error("pdgId() error");}
        virtual int status() const override{throw std::runtime_error("status() error");}
        virtual Particle mother() const override{throw std::runtime_error("mother() error");}
        virtual Particle daughter(int i) const override{throw std::runtime_error("daughter() error");}
        virtual int numberOfDaughters() const override{throw std::runtime_error("numberOfDaughters() error");}
        virtual Particle::Type getType() const  override{ return particleType; }
        virtual bool isNotNull() const override {return true;}
        virtual bool isFinalState() const override {return true;}
        virtual bool operator== (const ParticleImplementation& other) const override;
        // virtual Particle::BarrelState getBarrelState() const override {throw std::runtime_error("error");}
        // virtual bool isIsolated() const  override{throw std::runtime_error("error");};


    private:
        reco::Candidate::LorentzVector lorentzVec;
        const int particleCharge;
        Particle::Type particleType;
        const double particleIsolation;
};

#endif