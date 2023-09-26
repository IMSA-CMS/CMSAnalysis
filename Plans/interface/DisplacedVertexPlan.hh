#ifndef DISPLACEDVERTEXPLAN_HH
#define DISPLACEDVERTEXPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

//Contains the specification for all the module to run for a Displaced Vertex analysis
class DisplacedVertexPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif