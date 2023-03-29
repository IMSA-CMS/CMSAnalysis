#ifndef LEPTONJETSELECTOR_HH
#define LEPTONJETSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/Filters/interface/Selector.hh"

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class LeptonJetSelector : public Selector
{
    public:
        //Selects particles, keeping only loose muons with pT greater than 5.
        void selectParticles(const InputModule* input, Event& event) override;
};

#endif
