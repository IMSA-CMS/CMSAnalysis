#ifndef MLVARIABLESSPLITPLAN_HH
#define MLVARIABLESSPLITPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

// A plan for seperating data into lepton jets and recording values for each of these jets like phi,eta,pt,deltaR, etc.
class MLVariablesSplitPlan : public DataCollectionPlan
{
private:
    virtual void initialize() override;
};

#endif