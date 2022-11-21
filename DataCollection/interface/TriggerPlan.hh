#ifndef TRIGGERPLAN_HH
#define TRIGGERPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class TriggerPlan : public DataCollectionPlan
{
    public:
        TriggerPlan();
    protected:
        virtual void initialize() override;
    private:
        
};

#endif