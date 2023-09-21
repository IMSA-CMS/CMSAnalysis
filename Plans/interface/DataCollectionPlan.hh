#ifndef DATACOLLECTIONPLAN_HH
#define DATACOLLECTIONPLAN_HH

#include "ModuleCollection.hh"
#include "ModuleOptions.hh"


// Interface for data collection plans. Contains an modules, which is modified 
// by the constructor of each child class. Used within runmodules to conduct analyses.
class DataCollectionPlan
{
    public:
        // Runs the eventLoader with the specified parameters. 
        void runEventLoader(const std::string& inputFile, const std::string& outputFile, 
            int outputEvery = 0, int numFiles = -1, int maxEvents = -1);
        //void runmodulesWrapper(edm::Event event, const std::string& outputFile);

        void getOptions(std::shared_ptr<ModuleOptions> options) {moduleOptions = options;};

        virtual ~DataCollectionPlan(){}
        virtual void initialize() = 0;
       
        const ModuleCollection& getModules() const {return modules;}

        // Returns the modules as a reference, typically for use within a ChildPlan constructor
 
        ModuleCollection& getModules() {return modules;}

        std::shared_ptr<ModuleOptions> moduleOptions;
    private:
        ModuleCollection modules;
       };

#endif