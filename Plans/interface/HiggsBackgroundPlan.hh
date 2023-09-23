#ifndef HIGGSBACKGROUNDPLAN_HH
#define HIGGSBACKGROUNDPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class HiggsBackgroundPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif