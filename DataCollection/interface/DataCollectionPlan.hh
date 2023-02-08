#ifndef DATACOLLECTIONPLAN_HH
#define DATACOLLECTIONPLAN_HH


#include "FWCore/Framework/interface/Event.h"
#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"
#include "CMSAnalysis/DataCollection/interface/ModuleOptions.hh"


// Interface for data collection plans. Contains an analyzer, which is modified 
// by the constructor of each child class. Used within runAnalyzer to conduct analyses.
class DataCollectionPlan
{
    public:
        // Runs the eventLoader with the specified parameters. 
        void runEventLoader(const std::string& inputFile, const std::string& outputFile, 
            int outputEvery = 0, int numFiles = -1);
        //void runAnalyzerWrapper(edm::Event event, const std::string& outputFile);

        void getOptions(std::shared_ptr<ModuleOptions> options) {moduleOptions = options;};

        virtual ~DataCollectionPlan(){}
        virtual void initialize() = 0;
       
        const Analyzer& getAnalyzer() const {return analyzer;}

        // Returns the analyzer as a reference, typically for use within a ChildPlan constructor
 
        Analyzer& getAnalyzer() {return analyzer;}

        std::shared_ptr<ModuleOptions> moduleOptions;
    private:
        Analyzer analyzer;
        std::vector<std::string> fetchRootFiles(const std::string &configFile) const;
};

#endif