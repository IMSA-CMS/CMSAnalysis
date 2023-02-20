#ifndef EVENTLOADER_HH
#define EVENTLOADER_HH

#include "ParticleCollection.hh"
#include "EventFile.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "RootEventInterface.hh"
#include "Analyzer.hh"

class TFile;
class EventFile;
class Analyzer;

//Identifies the type of a root file and creates the appropriate implementation
class EventLoader
{
    public:
        //Constructor
        EventLoader(std::vector<std::string> fileList, Analyzer *nAnalyzer){rootFiles = fileList; analyzer = nAnalyzer;}

        //Changes file to approproate format
        std::shared_ptr<EventFile> changeFileFormat(TFile* ifile);
        void initialize();

        //Advances to next file
        void nextFile(std::shared_ptr<EventFile> newFile) {file=newFile;}

        //Calls process Root files
        void run(int outputEvery, int nFiles);

    private:
        void beginJob();
        void analyze(const RootEventInterface);
        void endJob();
        
        void processRootFiles(int outputEvery, int nFiles);

        std::vector<std::string> rootFiles;
        Analyzer *analyzer; 
        std::shared_ptr<EventFile> file = nullptr;
        RootEventInterface eventInterface = RootEventInterface();
        
};




#endif
