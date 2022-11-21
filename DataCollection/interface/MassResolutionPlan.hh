#ifndef MASSRESOLUTION_HH
#define MASSRESOLUTION_HH

#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

class MassResolutionPlan : public DataCollectionPlan
{
    public:
        MassResolutionPlan();
    
    protected:
        virtual void initialize() override;

    private:
        
};

#endif