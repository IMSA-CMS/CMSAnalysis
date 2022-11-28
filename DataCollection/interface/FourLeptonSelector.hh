#ifndef FOURLEPTONSELECTOR_HH
#define FOURLEPTONSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/Selector.hh"

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class FourLeptonSelector : public Selector
{
    public:
        FourLeptonSelector() {}
        void selectParticles(const InputModule* input, Event& event) override;
};

#endif