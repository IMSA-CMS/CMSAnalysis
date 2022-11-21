#ifndef MASSACCEPTANCEPLAN_HH
#define MASSACCEPTANCEPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class MassAcceptancePlan : public DataCollectionPlan
{
    public:
        MassAcceptancePlan();
    protected:
        virtual void initialize() override;
    private:
        
};

#endif