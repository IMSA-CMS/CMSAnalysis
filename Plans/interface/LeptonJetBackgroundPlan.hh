#ifndef LEPTONJETBACKGROUNDPLAN_HH
#define LEPTONJETBACKGROUNDPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class LeptonJetBackgroundPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif