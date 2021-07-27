#ifndef MINIAODEVENTLOADER_HH
#define MINIAODEVENTLOADER_HH

#include "EventLoader.hh"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/FWLite/interface/Event.h"

class MiniAODEventLoader : public EventLoader
{
    public:
        MiniAODEventLoader();
        // maybe just ++event
        virtual void nextEvent() override;
        
        virtual bool isDone() override;
    protected:
        virtual void newFile(TFile* ifile) override;
    private:
        fwlite::Event event;
        int numOfEvents;
        int outputEvery;
};



#endif