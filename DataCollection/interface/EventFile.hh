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
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const = 0;
        virtual ParticleCollection<Particle> getRecoParticles() const = 0;
        virtual ParticleCollection<Particle> getRecoJets() const = 0;
        //These two functions should be removed at some point
        virtual double getMET() const = 0;
        virtual int getNumPileUpInteractions() const = 0;
        TFile* getFile() {return file;}
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const = 0;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const = 0;
        int getNumOfEvents() const {return numOfEvents;}
        int getEventCount() const {return eventCount;}
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const = 0; //checks if event passes trigger criteria
    protected:
        void setNumOfEvents(int newNum) {numOfEvents = newNum;}
        void setEventCount(int newNum) {eventCount = newNum;}
    private:
        TFile* file;
        int numOfEvents;
        int eventCount;
};

#endif