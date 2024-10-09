#ifndef COUNTERPLAN_HH
#define COUNTERPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class CounterPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif