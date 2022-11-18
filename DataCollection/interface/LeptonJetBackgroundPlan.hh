#ifndef LEPTONJETBACKGROUNDPLAN_HH
#define LEPTONJETBACKGROUNDPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class LeptonJetBackgroundPlan : public DataCollectionPlan
{

    protected:
        void setupAnalyzer() override;
};

#endif