#ifndef EVENTLOADER_HH
#define EVENTLOADER_HH

#include "ParticleCollection.hh"
#include "EventFile.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"



class TFile;

class EventLoader
{
    public:
        EventLoader(int outputEvery) : outputEvery(outputEvery) {}
        void changeFile(TFile* ifile);
        std::shared_ptr<EventFile> getFile() const {return file;} 
        // simply dumps gensim particles from event into ParticleCollection
        //virtual std::vector<PileupSummaryInfo> getPileupInfo() const = 0;
        
        // virtual edm::TriggerResults getTriggerResults(std::string subProcess) const = 0;
        // virtual edm::TriggerNames getTriggerNames(std::string subProcess) const = 0;
        
    protected:
        //TFile* getFile() {return file;}
        //const TFile* getFile() const {return file;}
        //virtual void newFile(TFile* ifile) = 0;
    private:
        //TFile* file;
        int outputEvery;
        std::shared_ptr<EventFile> file;
};




#endif
