#ifndef GENSIMPLAN_HH
#define GENSIMPLAN_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class GenSimPlan : public DataCollectionPlan
{
    public:
        GenSimPlan();
    protected:
        virtual void initialize() override; 
};

#endif