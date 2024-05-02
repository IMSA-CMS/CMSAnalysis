#ifndef HIGGSSIGNALPLAN_HH
#define HIGGSSIGNALPLAN_HH

#include "CMSAnalysis/Plans/interface/HiggsBackgroundPlan.hh"

class HiggsSignalPlan : public HiggsBackgroundPlan
{
    protected:
        virtual void initialize() override;
};

#endif