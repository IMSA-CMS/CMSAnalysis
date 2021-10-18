#ifndef MINIAODEVENTLOADER_HH
#define MINIAODEVENTLOADER_HH

#include "EventLoader.hh"
#include "ParticleCollection.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"

class MiniAODEventLoader : public EventLoader
{
    public:
        MiniAODEventLoader(int outputEvery);
        // maybe just ++event
        virtual void nextEvent() override;
        
        virtual bool isDone() const override;
    protected:
        virtual void newFile(TFile* ifile) override;
        //virtual void getLeptons(RecoLevel level) override;
        virtual ParticleCollection getGenSimParticles() const override;
        virtual ParticleCollection getRecoParticles() const override;
        //virtual std::vector<PileupSummaryInfo> getPileupInfo() const override;
        virtual GenEventInfoProduct getGenInfo() const override;
        virtual double getMET() const override;
    private:
        std::shared_ptr<fwlite::Event> event = nullptr;
};



#endif