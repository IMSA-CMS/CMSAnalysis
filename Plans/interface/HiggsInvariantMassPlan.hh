#ifndef HIGGSINVARIANTMASSPLAN_HH
#define HIGGSINVARIANTMASSPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class HiggsInvariantMassPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif