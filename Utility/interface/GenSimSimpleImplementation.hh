#ifndef GENSIMSIMPLEIMPLEMENTATION_HH
#define GENSIMSIMPLEIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleImplementation.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"


class GenSimParticle;

class GenSimSimpleImplementation : public ParticleImplementation
{
    public:
        virtual ~GenSimSimpleImplementation(){}
        GenSimSimpleImplementation(reco::Candidate::LorentzVector vec, int ch, const ParticleType& type, const Particle* motherParticle, std::vector<const GenSimParticle*> daughters, const int status);
        virtual reco::Candidate::LorentzVector getFourVector() const override;
        //virtual double dxy() const {throw std::runtime_error("getDxy not implemented");}
        //virtual double dz() const {throw std::runtime_error("getDz not implemented");}
        virtual int charge() const override;
        int pdgId() const override;
        virtual int status() const override{ return particleStatus; }
        virtual Particle mother() const override {return *motherParticle;}
        virtual Particle daughter(int i) const override;
        virtual bool doesHaveMother() const override;
        virtual int numberOfDaughters() const override{return daughters.size();}
        virtual const ParticleType& getType() const  override{ return particleType; }
        virtual Particle::SelectionFit getSelectionFit() const override{throw std::runtime_error("selectionFit error");}
        virtual bool isNotNull() const override {return true;}
        virtual bool isFinalState() const override {return true;}
        virtual bool operator== (const ParticleImplementation& other) const override;
        //virtual double isolation() const override{throw std::runtime_error("isolation error");}

    private:
        reco::Candidate::LorentzVector lorentzVec;
        const int particleCharge;
        const ParticleType particleType;
        const Particle* motherParticle;
        std::vector<const GenSimParticle*> daughters;
        const int particleStatus;
};

#endif