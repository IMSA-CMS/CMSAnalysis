#ifndef SIMPLEIMPLEMENTATION_HH
#define SIMPLEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleImplementation.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"

class SimpleImplementation : public ParticleImplementation
{
    public:
        virtual ~SimpleImplementation(){}
        SimpleImplementation(reco::Candidate::LorentzVector vec, double dxy, double dz, int ch, const ParticleType& type, 
            Particle::SelectionFit fit = Particle::SelectionFit::Tight);

        virtual reco::Candidate::LorentzVector getFourVector() const override;
        virtual int charge() const override;

        // virtual int pdgId() const override{ throw std::runtime_error("pdgId() error");}
        virtual int pdgId() const override{ return getType().getpdgId(); }
        virtual int status() const override{throw std::runtime_error("status() error");}
        virtual Particle mother() const override{throw std::runtime_error("mother() error");}
        virtual Particle daughter(int i) const override{throw std::runtime_error("daughter() error");}
        virtual bool doesHaveMother() const override {throw std::runtime_error("not implemented error");}
        virtual int numberOfDaughters() const override{throw std::runtime_error("numberOfDaughters() error");}
        virtual const ParticleType& getType() const  override{ return particleType; }
        virtual Particle::SelectionFit getSelectionFit() const override{ return selectionFit;}
        inline virtual double getDXY() const override { return deltaXY; }
        inline virtual double getDZ() const override { return deltaXY; }
        virtual bool isNotNull() const override {return true;}
        virtual bool isFinalState() const override {return true;}
        virtual bool operator== (const ParticleImplementation& other) const override;
        // virtual Particle::BarrelState getBarrelState() const override {throw std::runtime_error("error");}
        // virtual bool isIsolated() const  override{throw std::runtime_error("error");};


    private:
        reco::Candidate::LorentzVector lorentzVec;
        double deltaXY;
        double deltaZ;
        const int particleCharge;
        const ParticleType& particleType;
        Particle::SelectionFit selectionFit;
};

#endif