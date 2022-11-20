#ifndef MASSACCEPTANCEPLAN_HH
#define MASSACCEPTANCEPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class MassAcceptancePlan : public DataCollectionPlan
{
    protected:
        void setupAnalyzer() override;
};

#endif