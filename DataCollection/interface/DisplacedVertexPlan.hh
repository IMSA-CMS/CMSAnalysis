#ifndef DISPLACEDVERTEXPLAN_HH
#define DISPLACEDVERTEXPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class DisplacedVertexPlan : public DataCollectionPlan
{
    public:
        DisplacedVertexPlan();
    protected:
        virtual void initialize() override;
};

#endif