#ifndef LEPTONJETBACKGROUNDPLAN_HH
#define LEPTONJETBACKGROUNDPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class LeptonJetBackgroundPlan : public DataCollectionPlan
{
    public:
        LeptonJetBackgroundPlan();
    protected:
        virtual void initialize() override;
};

#endif