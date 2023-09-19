#ifndef DATACOLLECTIONPLAN_HH
#define DATACOLLECTIONPLAN_HH

#include "ModuleCollection.hh"
#include "ModuleOptions.hh"


// Interface for data collection plans. Contains an analyzer, which is modified 
// by the constructor of each child class. Used within runAnalyzer to conduct analyses.
class DataCollectionPlan
{
    public:
        // Runs the eventLoader with the specified parameters. 
        void runEventLoader(const std::string& inputFile, const std::string& outputFile, 
            int outputEvery = 0, int numFiles = -1, int maxEvents = -1);
        //void runAnalyzerWrapper(edm::Event event, const std::string& outputFile);

        void getOptions(std::shared_ptr<ModuleOptions> options) {moduleOptions = options;};

        virtual ~DataCollectionPlan(){}
        virtual void initialize() = 0;
       
        const ModuleCollection& getAnalyzer() const {return modules;}

        // Returns the analyzer as a reference, typically for use within a ChildPlan constructor
 
        ModuleCollection& getAnalyzer() {return modules;}

        std::shared_ptr<ModuleOptions> moduleOptions;
    private:
        ModuleCollection modules;
       };

#endif