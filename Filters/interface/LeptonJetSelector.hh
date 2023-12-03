#ifndef LEPTONJETSELECTOR_HH
#define LEPTONJETSELECTOR_HH
#include <memory>
#include <vector>
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

#include "CMSAnalysis/Filters/interface/Selector.hh"

class LeptonJetReconstructionModule;

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class LeptonJetSelector : public Selector
{
    public:
        //Selects particles, keeping only loose muons with pT greater than 5.
        void selectParticles(const EventInput* input, Event& event) const override;
        LeptonJetSelector(double deltaRCut = .5);

    private:
        std::vector<LeptonJet> findLeptonJets(ParticleCollection<Lepton> recoCandidates) const;
        LeptonJet createLeptonJet(Lepton highestPtLepton) const;
        Particle findHighestPtLepton(std::vector<Lepton> leptons) const;
        double deltaRCut;
};

#endif
