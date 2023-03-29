#ifndef DELPHESIMPLEMENTATION_HH
#define DELPHESIMPLEMENTATION_HH

#include <vector>
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleImplementation.hh"

class DelphesImplementation : public ParticleImplementation
{
    // Allows GenSimEventDump to run Delphes Files
    public:
        // Bbunch of getter methods, returning various information such as charge, ID, # of daughters, etc.
        virtual ~DelphesImplementation(){}
        DelphesImplementation(reco::Candidate::LorentzVector vec, int charge, const ParticleType& type, int pid, int status, int m1, int m2,int d1, int d2,double iIsolation = -999);
        virtual reco::Candidate::LorentzVector getFourVector() const override;
        virtual int charge() const override{return particleCharge;}
        virtual double isolation() const override {return particleIsolation;}

        virtual double dxy() const override {throw std::runtime_error("dxy not implemented for this particle implementation");}
        virtual double dz() const {throw std::runtime_error("dz not implemented for this particle implementation");}

        virtual int pdgId() const override{return particleID;}
        virtual int status() const override{return statusCode;}
        virtual Particle mother() const override{throw std::runtime_error("mother() error");}
        virtual Particle daughter(int i) const override{throw std::runtime_error("daughter() error");}
        virtual std::vector<int> mothers() const{return mothersVec;}
        virtual std::vector<int> daughters() const{return daughtersVec;}
        virtual int numberOfDaughters() const override{return daughtersVec.size();}
        virtual const ParticleType& getType() const  override{ return particleType; }
        virtual Particle::SelectionFit getSelectionFit() const override {throw std::runtime_error("getSelectionFit not implemented");};
        virtual bool isNotNull() const override {return true;}
        virtual bool isFinalState() const override {return status()==1;}
        virtual bool operator== (const ParticleImplementation& other) const override;


    private:
        reco::Candidate::LorentzVector lorentzVec;
        const int particleCharge;
        const ParticleType& particleType;
        const double particleIsolation;
        const int particleID;
        const int statusCode;
        std::vector<int> mothersVec;
        std::vector<int> daughtersVec;
};

#endif