#ifndef CMSSWEVENTINTERFACE_HH
#define CMSSWEVENTINTERFACE_HH

#include "EventLoader.hh"
#include "ParticleCollection.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"

//class TFile;

class CmsswEventInterface : public EventInterface
{
    public:
        CmsswEventInterface();
        // maybe just ++event
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const override;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const override;
        void setEvent(edm::Event* newEvent){event = newEvent;}
        //virtual void getLeptons(RecoLevel level) override;
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const override;
        virtual ParticleCollection<Particle> getRecoParticles() const override;
        virtual ParticleCollection<Particle> getRecoJets() const override;
        //virtual std::vector<PileupSummaryInfo> getPileupInfo() const override;
        virtual double getMET() const override;
        
    private:
        edm::Event* event = nullptr;
};



#endif
