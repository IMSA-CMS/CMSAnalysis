#ifndef LEPTONJETSELECTOR_HH
#define LEPTONJETSELECTOR_HH
#include <memory>

#include "CMSAnalysis/Filters/interface/Selector.hh"

class LeptonJetReconstructionModule;

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class LeptonJetSelector : public Selector
{
    public:
        //Selects particles, keeping only loose muons with pT greater than 5.
        void selectParticles(const InputModule* input, Event& event) const override;

    private:
        std::shared_ptr<LeptonJetReconstructionModule> recoMod;
};

#endif
