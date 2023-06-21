#ifndef PASSELECTOR_HH
#define PASSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

class Particle;
class Lepton;

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class PASSelector : public Selector
{
    public:
        ~PASSelector() override {}
        //Keeps tight electrons and muons with pT greater than 40 and eta between -3 and 3 (muons) and -2.8 and 2.8 (electrons)
        void selectParticles(const EventInput* input, Event& event) const override;
};

#endif