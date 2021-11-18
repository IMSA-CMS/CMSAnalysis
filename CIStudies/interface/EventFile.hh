#ifndef EVENTFILE_HH
#define EVENTFILE_HH

#include "ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

class TFile;

class EventFile
{
    public:
        EventFile(TFile* ifile): file(ifile) {}
        virtual ~EventFile() {}
        virtual void nextEvent() = 0;
        virtual bool isDone() const = 0;
        virtual ParticleCollection getGenSimParticles() const = 0;
        virtual ParticleCollection getRecoParticles() const = 0;
        virtual ParticleCollection getRecoJets() const = 0;
        //These two functions should be removed at some point
        virtual GenEventInfoProduct getGenInfo() const = 0;
        virtual double getMET() const = 0;
        TFile* getFile() {return file;}
    protected:
        int getNumOfEvents() const {return numOfEvents;}
        void setNumOfEvents(int newNum) {numOfEvents = newNum;}
        int getEventCount() const {return eventCount;}
        void setEventCount(int newNum) {eventCount = newNum;}
    private:
        TFile* file;
        int numOfEvents;
        int eventCount;
};

#endif