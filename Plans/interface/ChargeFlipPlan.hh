#ifndef CHARGEFLIPPLAN__HH
#define CHARGEFLIPPLAN__HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class ChargeFlipPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif