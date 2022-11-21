#ifndef INVARIANTMASSPLAN_HH
#define INVARIANTMASSPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class InvariantMassPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif