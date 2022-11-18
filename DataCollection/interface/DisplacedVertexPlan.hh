#ifndef DISPLACEDVERTEXPLAN_HH
#define DISPLACEDVERTEXPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class DisplacedVertexPlan : public DataCollectionPlan
{
    protected:
        void setupAnalyzer() override;
};

#endif