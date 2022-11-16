#ifndef SIMPLEIMPLEMENTATION_HH
#define SIMPLEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"

class SimpleImplementation : public ParticleImplementation
{
    public:
        virtual ~SimpleImplementation(){}
        SimpleImplementation(reco::Candidate::LorentzVector vec, int ch, const ParticleType& type, double iIsolation = -999, 
            Particle::SelectionFit fit = Particle::SelectionFit::Tight);

        //added for dxy, dz
        SimpleImplementation(reco::Candidate::LorentzVector vec, int ch, const ParticleType& type, double iIsolation = -999,
            Particle::SelectionFit fit = Particle::SelectionFit::Tight, double dxy = 0, double dz = 0);

        virtual reco::Candidate::LorentzVector getFourVector() const override;
        virtual int charge() const override;
        virtual double isolation() const override {return particleIsolation;}

        virtual double dxy() const override {return particleDxy;}
        virtual double dz() const {return particleDz;}

        virtual int pdgId() const override{throw std::runtime_error("pdgId() error");}
        virtual int status() const override{throw std::runtime_error("status() error");}
        virtual Particle mother() const override{throw std::runtime_error("mother() error");}
        virtual Particle daughter(int i) const override{throw std::runtime_error("daughter() error");}
        virtual int numberOfDaughters() const override{throw std::runtime_error("numberOfDaughters() error");}
        virtual const ParticleType& getType() const  override{ return particleType; }
        virtual Particle::SelectionFit getSelectionFit() const override{ return selectionFit;}
        virtual bool isNotNull() const override {return true;}
        virtual bool isFinalState() const override {return true;}
        virtual bool operator== (const ParticleImplementation& other) const override;
        // virtual Particle::BarrelState getBarrelState() const override {throw std::runtime_error("error");}
        // virtual bool isIsolated() const  override{throw std::runtime_error("error");};


    private:
        reco::Candidate::LorentzVector lorentzVec;
        const int particleCharge;
        const ParticleType& particleType;
        const double particleIsolation;
        Particle::SelectionFit selectionFit;
        double particleDxy;
        double particleDz;
};

#endif