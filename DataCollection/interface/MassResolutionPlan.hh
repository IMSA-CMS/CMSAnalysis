#ifndef MASSRESOLUTION_HH
#define MASSRESOLUTION_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class MassResolutionPlan : public DataCollectionPlan
{
    protected:
        void setupAnalyzer() override;
};

#endif