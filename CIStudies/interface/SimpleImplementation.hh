#ifndef SIMPLEIMPLEMENTATION_HH
#define SIMPLEIMPLEMENTATION_HH

#include <vector>
#include "CIAnalysis/CIStudies/interface/Particle.hh"
#include "CIAnalysis/CIStudies/interface/ParticleImplementation.hh"

class SimpleImplementation : public ParticleImplementation
{
    public:
        virtual ~SimpleImplementation(){}
        SimpleImplementation(const reco::Candidate::LorentzVector*);
        virtual reco::Candidate::LorentzVector getFourVector() const override;
        virtual int charge() const override;
        virtual double et() const override {throw std::runtime_error("error");}
        virtual double energy() const override{throw std::runtime_error("error");}
        virtual int pdgId() const override{throw std::runtime_error("error");}
        virtual int status() const override{throw std::runtime_error("error");}
        virtual Particle mother() const override{throw std::runtime_error("error");}
        virtual Particle daughter(int i) const override{throw std::runtime_error("error");}
        virtual int numberOfDaughters() const override{throw std::runtime_error("error");}
        virtual Particle::Type getType() const  override{throw std::runtime_error("error");}
        virtual bool isNotNull() const override {return true;}
        // virtual Particle::BarrelState getBarrelState() const override {throw std::runtime_error("error");}
        // virtual bool isIsolated() const  override{throw std::runtime_error("error");};


    private:
        reco::Candidate::LorentzVector* lorentzVec;
        const reco::Candidate* particle;
        virtual void checkIsNull() const;
};

#endif