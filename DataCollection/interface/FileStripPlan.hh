#ifndef FILESTRIPPLAN_HH
#define FILESTRIPPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

// A class that calls FileStripModule, and works with runAnalyzer. 
// To use file stripping, call the FileStrip plan while running runAnalyzer, and 
// change the name of the stripped file in FileStripPlan.cc
class FileStripPlan : public DataCollectionPlan
{
    protected:
        void setupAnalyzer() override;
};

#endif