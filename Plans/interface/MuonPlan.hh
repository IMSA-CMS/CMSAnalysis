#ifndef MUONPLAN_HH
#define MUONPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class MuonPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif