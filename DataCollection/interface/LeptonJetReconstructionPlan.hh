#ifndef LEPTONJETRECONSTRUCTIONPLAN_HH
#define LEPTONJETRECONSTRUCTIONPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class LeptonJetReconstructionPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif