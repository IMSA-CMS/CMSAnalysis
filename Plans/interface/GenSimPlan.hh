#ifndef GENSIMPLAN_HH
#define GENSIMPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

//Contains the specification for all the module to run on the analysis of a GenSim File
class GenSimPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override; 
};

#endif