#ifndef LEPTONJETSELECTOR_HH
#define LEPTONJETSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class LeptonJetSelector : public Selector
{
    public:
        //Selects particles, keeping only loose muons with pT greater than 5.
        void selectParticles(const InputModule* input, Event& event) override;

    private:
        std::shared_ptr<LeptonJetReconstructionModule> recoMod;
};

#endif
