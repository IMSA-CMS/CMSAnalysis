#ifndef MUONSIGNALPLAN_HH
#define MUONSIGNALPLAN_HH

#include "CMSAnalysis/Plans/interface/MuonPlan.hh"

class MuonSignalPlan : public MuonPlan
{
    protected:
        virtual void initialize() override;
};

#endif