#ifndef TRIGGERPLAN_HH
#define TRIGGERPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class TriggerPlan : public DataCollectionPlan
{
    protected:
        void setupAnalyzer() override;
};

#endif