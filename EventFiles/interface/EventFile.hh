#ifndef EVENTFILE_HH
#define EVENTFILE_HH

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "CMSAnalysis/DataCollection/interface/EventInterface.hh"

class TFile;
class FileParams;

//EventFile class is a container for a Tfile for events
class EventFile
{
    public:
        EventFile(TFile* ifile, std::shared_ptr<FileParams> iparams): file(ifile), params(iparams) {}
        virtual bool isDone() const = 0;
        virtual ~EventFile() {}
        virtual ParticleCollection<GenSimParticle> getGenSimParticles() const = 0;
        virtual ParticleCollection<Particle> getRecoParticles() const = 0;
        virtual ParticleCollection<Particle> getRecoJets() const = 0;
        virtual int getNumOfEvents() const = 0;
        int getEventCount() const {return eventCount;}
        virtual reco::Candidate::LorentzVector getMET() const = 0;
        virtual unsigned long long getEventIDNum() const = 0; 
        virtual long getRunNum() const = 0; 
        virtual int getLumiBlock() const = 0;
        virtual int getNumPileUpInteractions() const = 0;
        TFile* getFile() {return file;}
        virtual bool checkTrigger(std::string triggerName, std::string subProcess) const = 0; //checks if event passes trigger criteria
        virtual void nextEvent()=0;
        virtual std::vector<double> getPDFWeights() const = 0;
        virtual std::vector<bool> getTriggerResults(std::string subProcess) const = 0;
        virtual std::vector<std::string> getTriggerNames(std::string subProcess) const = 0;
        const std::shared_ptr<FileParams> getFileParams() const {return params;}
    protected:
        void setEventCount(int newNum) {eventCount = newNum;}

    private:
        TFile* file;
        int eventCount;
        const std::shared_ptr<FileParams> params;
        
};

// class EventFile
// {
//     public:
//         
//         virtual void nextEvent() = 0;
//         virtual bool isDone() const = 0;
//         virtual ParticleCollection<GenSimParticle> getGenSimParticles() const = 0;
//         virtual ParticleCollection<Particle> getRecoParticles() const = 0;
//         virtual ParticleCollection<Particle> getRecoJets() const = 0;
//         //These two functions should be removed at some point
//         virtual double getMET() const = 0;
//         TFile* getFile() {return file;}
//         virtual std::vector<bool> getTriggerResults(std::string subProcess) const = 0;
//         virtual std::vector<std::string> getTriggerNames(std::string subProcess) const = 0;
//         int getNumOfEvents() const {return numOfEvents;}
//         int getEventCount() const {return eventCount;}
//     protected:
//         void setNumOfEvents(int newNum) {numOfEvents = newNum;}
//         void setEventCount(int newNum) {eventCount = newNum;}
//     private:
//         TFile* file;
//         int numOfEvents;
//         int eventCount;
// };

#endif