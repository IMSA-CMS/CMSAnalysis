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

        //responsible for outputing a short message every n events
        void setOutputEvery(int output) {outputEvery = output;}

        //Calls process Root files
        void run(int outputEvery, int nFiles);

    private:
        void beginJob();
        void analyze(const RootEventInterface);
        void endJob();
        
        void processRootFiles(int outputEvery, int nFiles);

        int outputEvery = 0;
        std::vector<std::string> rootFiles;
        Analyzer *analyzer; 
        std::shared_ptr<EventFile> file = nullptr;
        RootEventInterface eventInterface = RootEventInterface();
        
};




#endif
