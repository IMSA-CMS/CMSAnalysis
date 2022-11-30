#ifndef EVENTLOADER_HH
#define EVENTLOADER_HH

#include "ParticleCollection.hh"
#include "EventFile.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

class TFile;

//Identifies the type of a root file and creates the appropriate implementation
class EventLoader
{
    public:
        //Identifies the proper type and sets the file to that implementation
        void changeFileFormat(TFile* ifile);

        std::shared_ptr<EventFile> getFile() const {return file;} 

        //Responsible for outputing a short message after processing a certain ammount of events
        void setOutputEvery(int output) {outputEvery = output;}

    private:
        int outputEvery = 0;
        std::shared_ptr<EventFile> file;
};




#endif
