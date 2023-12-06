#ifndef LeptonJetAntiSelector_HH
#define LeptonJetAntiSelector_HH
#include <memory>
#include <vector>
<<<<<<< HEAD
=======
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
>>>>>>> 4bf776d4a82ffdc1e0fa6d26886598b0215cc0df

#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class LeptonJetAntiSelector : public Selector
{
    public:
        LeptonJetAntiSelector(double ideltaRCut = .5, double idXYCut = 0, double idZCut = 0);

        //Selects particles for a control region, keeping only loose muons with pT greater than 5 that have a dXY and dZ greater than the given values.
        void selectParticles(const EventInput* input, Event& event) const override;
    private:
        std::vector<LeptonJet> findLeptonJets(ParticleCollection<Lepton> recoCandidates) const;
        LeptonJet createLeptonJet(Lepton highestPtLepton) const;
        Particle findHighestPtLepton(std::vector<Lepton> leptons) const;

        double deltaRCut, dXYCut, dZCut;
};

#endif
