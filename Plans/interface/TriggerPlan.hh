#ifndef TRIGGERPLAN_HH
#define TRIGGERPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class TriggerPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
    private:
        
};

#endif