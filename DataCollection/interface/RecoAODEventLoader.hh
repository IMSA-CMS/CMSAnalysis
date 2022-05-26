#ifndef RECOAODEVENTLOADER_HH
#define RECOAODEVENTLOADER_HH

#include "EventLoader.hh"

class RecoAODEventLoader : public EventLoader
{
    public:
        RecoAODEventLoader();

        virtual void nextEvent() override;
        
        virtual bool isDone() override;
    protected:
        
        virtual void newFile(TFile* ifile) override;
    private:
        fwlite::Event event;
};

#endif