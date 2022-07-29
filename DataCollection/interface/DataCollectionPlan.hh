#ifndef DATACOLLECTIONPLAN_HH
#define DATACOLLECTIONPLAN_HH

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"


// Interface for data collection plans. Contains an analyzer, which is modified 
// by the constructor of each child class. Used within runAnalyzer to conduct analyses.
class DataCollectionPlan
{
    public:
        // Runs the analyzer with the specified parameters. 
        void runAnalyzer(const std::string& inputFile, const std::string& outputFile, 
            int outputEvery = 0, int numFiles = -1);
            
    protected:
        const Analyzer& getAnalyzer() const {return analyzer;}

        // Returns the analyzer as a reference, typically for use within a ChildPlan constructor
        Analyzer& getAnalyzer() {return analyzer;}
    private:
        Analyzer analyzer;
};

#endif