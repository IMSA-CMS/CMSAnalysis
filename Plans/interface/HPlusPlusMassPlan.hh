#ifndef HPLUSPLUSMASSPLAN_HH
#define HPLUSPLUSMASSPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class HPlusPlusMassPlan : public DataCollectionPlan
{
    protected:
        void initialize() override;
};

#endif