#ifndef HIGGSCUTSSELECTOR_HH
#define HIGGSCUTSSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/Selector.hh"

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class HiggsCutsSelector : public Selector
{
    public:
        HiggsCutsSelector() {}
        void selectParticles(const InputModule* input, Event& event) override;
};

#endif