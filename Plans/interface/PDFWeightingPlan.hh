#ifndef PDFWEIGHTINGPLAN_HH
#define PDFWEIGHTINGPLAN_HH

#include "CMSAnalysis/Plans/interface/DataCollectionPlan.hh"

//Contains the specification for all the module to run for a Displaced Vertex analysis
class PDFWeightingPlan : public DataCollectionPlan
{
    protected:
        virtual void initialize() override;
};

#endif