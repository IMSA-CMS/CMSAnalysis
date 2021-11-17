#ifndef EVENTLOADER_HH
#define EVENTLOADER_HH

#include "ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"


class TFile;

class EventLoader
{
    public:
        EventLoader(int outputEvery) : numOfEvents(0), eventCount(0), outputEvery(outputEvery) {}
        virtual ~EventLoader() {}
        void changeFile(TFile* jfile) {file = jfile; newFile(jfile);}
        virtual void nextEvent() = 0;
        virtual bool isDone() const = 0;
        // simply dumps gensim particles from event into ParticleCollection
        virtual ParticleCollection getGenSimParticles() const = 0;
        virtual ParticleCollection getRecoParticles() const = 0;
        //virtual std::vector<PileupSummaryInfo> getPileupInfo() const = 0;
        virtual GenEventInfoProduct getGenInfo() const = 0;
        virtual edm::Handle<edm::TriggerResults> getTriggerResults(std::string subProcess) const = 0;
        virtual edm::TriggerNames getTriggerNames(std::string subProcess) const = 0;
        virtual double getMET() const = 0;
    protected:
        TFile* getFile() {return file;}
        const TFile* getFile() const {return file;}
        virtual void newFile(TFile* ifile) = 0;
        int getNumOfEvents() const {return numOfEvents;}
        void setNumOfEvents(int newNum) {numOfEvents = newNum;}
        int getEventCount() const {return eventCount;}
        void setEventCount(int newNum) {eventCount = newNum;}
    private:
        TFile* file;
        int numOfEvents;
        int eventCount;
        int outputEvery;
};




#endif