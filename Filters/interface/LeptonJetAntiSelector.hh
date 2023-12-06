#ifndef LEPTONJETANTISELECTOR_HH
#define LEPTONJETANTISELECTOR_HH
#include <memory>
#include <vector>

#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class LeptonJetAntiSelector : public Selector
{
    public:
        LeptonJetAntiSelector(double idXYCut = 0, double idZCut = 0);

        //Selects particles for a control region, keeping only loose muons with pT greater than 5 that have a dXY and dZ greater than the given values.
        void selectParticles(const EventInput* input, Event& event) const override;
    private:
        static std::vector<LeptonJet> findLeptonJets(ParticleCollection<Lepton> recoCandidates, double DeltaRCut);
        static Particle findHighestPtLepton(std::vector<Lepton> particles);
        static LeptonJet createLeptonJet(Lepton highestPtLepton);

        double dXYCut, dZCut;
};

#endif
