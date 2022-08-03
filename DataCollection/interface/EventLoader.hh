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
        void changeFileFormat(TFile* ifile);
        std::shared_ptr<EventFile> getFile() const {return file;} 
        void setOutputEvery(int output) {outputEvery = output;}

    private:
        int outputEvery = 0;
        std::shared_ptr<EventFile> file;
};




#endif
