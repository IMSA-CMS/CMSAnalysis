#ifndef EVENTLOADER_HH
#define EVENTLOADER_HH

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/EventFiles/interface/EventFile.hh"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "RootEventInterface.hh"
#include "Analyzer.hh"
#include "CMSAnalysis/Utility/interface/FileParams.hh"

class TFile;
class EventFile;
class Analyzer;

//Identifies the type of a root file and creates the appropriate implementation
class EventLoader
{
    public:
        //Constructor
        EventLoader(std::vector<std::shared_ptr<FileParams>> fileList, Analyzer *nAnalyzer);

        //Changes file to approproate format
        std::shared_ptr<EventFile> changeFileFormat(TFile* ifile, std::shared_ptr<FileParams> iparams);
        void initialize();

        //Advances to next file
        void nextFile(std::shared_ptr<EventFile> newFile) {file=newFile;}

        //Calls process Root files
        void run(int outputEvery, int nFiles, int maxEvents);

        int getNumOfEvents() const {return numOfEvents;}

        static std::vector<std::shared_ptr<FileParams>> fetchRootFiles(const std::string &configFile);

    protected:
        int numOfEvents;

    private:
        void beginJob();
        void analyze(const RootEventInterface);
        void endJob();
        
        void processRootFiles(int outputEvery, int nFiles, int maxEvents);

        std::vector<std::shared_ptr<FileParams>> rootFiles;
        Analyzer *modules; 
        std::shared_ptr<EventFile> file = nullptr;
        RootEventInterface eventInterface = RootEventInterface();
        
};




#endif
