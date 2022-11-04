#ifndef GENSIMSIMPLEIMPLEMENTATION_HH
#define GENSIMSIMPLEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"


class GenSimParticle;

class GenSimSimpleImplementation : public ParticleImplementation
{
    public:
        virtual ~GenSimSimpleImplementation(){}
        GenSimSimpleImplementation(reco::Candidate::LorentzVector vec, int ch, const ParticleType& type, const Particle* motherParticle, std::vector<const GenSimParticle*> daughters);
        virtual reco::Candidate::LorentzVector getFourVector() const override;
        virtual int charge() const override;
        virtual int pdgId() const override{throw std::runtime_error("pdgID error");}
        virtual int status() const override{throw std::runtime_error("status() error");}
        virtual Particle mother() const override {return *motherParticle;}
        virtual Particle daughter(int i) const override;
        virtual int numberOfDaughters() const override{return daughters.size();}
        virtual const ParticleType& getType() const  override{ return particleType; }
        virtual Particle::SelectionFit getSelectionFit() const override{throw std::runtime_error("selectionFit error");}
        virtual bool isNotNull() const override {return true;}
        virtual bool isFinalState() const override {return true;}
        virtual bool operator== (const ParticleImplementation& other) const override;
        virtual double isolation() const override{throw std::runtime_error("isolation error");}

    private:
        reco::Candidate::LorentzVector lorentzVec;
        const int particleCharge;
        const ParticleType particleType;
        const Particle* motherParticle;
        std::vector<const GenSimParticle*> daughters;
};

#endif