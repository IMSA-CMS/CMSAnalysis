#ifndef DELPHESIMPLEMENTATION_HH
#define DELPHESIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleImplementation.hh"

class DelphesImplementation : public ParticleImplementation
{
    public:
        virtual ~DelphesImplementation(){}
        DelphesImplementation(reco::Candidate::LorentzVector vec, int charge, Particle::Type type, int pid, int status, int m1, int m2,int d1, int d2,double iIsolation = -999);
        virtual reco::Candidate::LorentzVector getFourVector() const override;
        virtual int charge() const override{return particleCharge;}
        virtual double et() const override {throw std::runtime_error("et() error");}
        virtual double energy() const override{throw std::runtime_error("energy() error");}
        virtual double isolation() const override {return particleIsolation;}
        virtual int pdgId() const override{return particleID;}
        virtual int status() const override{return statusCode;}
        virtual Particle mother() const override{throw std::runtime_error("mother() error");}
        virtual Particle daughter(int i) const override{throw std::runtime_error("daughter() error");}
        virtual std::vector<int> mothers() const{return mothersVec;}
        virtual std::vector<int> daughters() const{return daughtersVec;}
        virtual int numberOfDaughters() const override{return daughtersVec.size();}
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
        const int particleID;
        const int statusCode;
        std::vector<int> mothersVec;
        std::vector<int> daughtersVec;
};

#endif