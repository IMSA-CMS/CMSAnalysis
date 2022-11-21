#ifndef HIGGSBACKGROUNDPLAN_HH
#define HIGGSBACKGROUNDPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class HiggsBackgroundPlan : public DataCollectionPlan
{
    public:
        HiggsBackgroundPlan();
    protected:
        virtual void initialize() override;
};

#endif