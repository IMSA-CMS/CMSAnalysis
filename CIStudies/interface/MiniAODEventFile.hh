#ifndef MINIAODEVENTFILE_HH
#define MINIAODEVENTFILE_HH

#include "EventLoader.hh"
#include "ParticleCollection.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"

class TFile;

class MiniAODEventFile : public EventFile
{
    public:
        MiniAODEventFile(TFile* ifile);
        // maybe just ++event
        virtual void nextEvent() override;
        
        virtual bool isDone() const override;
    protected:
        //virtual void getLeptons(RecoLevel level) override;
        virtual ParticleCollection getGenSimParticles() const override;
        virtual ParticleCollection getRecoParticles() const override;
        //virtual std::vector<PileupSummaryInfo> getPileupInfo() const override;
        virtual GenEventInfoProduct getGenInfo() const override;
        virtual double getMET() const override;
        virtual edm::Handle<edm::TriggerResults> getTriggerResults(std::string subProcess) const override;
        virtual edm::TriggerNames getTriggerNames(std::string subProcess) const override;
    private:
        std::shared_ptr<fwlite::Event> event = nullptr;
};



#endif