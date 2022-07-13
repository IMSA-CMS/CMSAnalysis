#ifndef DATACOLLECTIONPLAN_HH
#define DATACOLLECTIONPLAN_HH

#include "CMSAnalysis/DataCollection/interface/Analyzer.hh"



class DataCollectionPlan
{
    public:
        void runAnalyzer(const std::string& inputFile, const std::string& outputFile, 
            int outputEvery = 0, int numFiles = -1);
    protected:
        const Analyzer& getAnalyzer() const {return analyzer;}
        Analyzer& getAnalyzer() {return analyzer;}
    private:
        Analyzer analyzer;
};

#endif