#ifndef LEPTONJETRECONSTRUCTIONPLAN_HH
#define LEPTONJETRECONSTRUCTIONPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

class LeptonJetReconstructionPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif