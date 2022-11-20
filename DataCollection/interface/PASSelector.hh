#ifndef PASSELECTOR_HH
#define PASSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

class Particle;
class Lepton;

//To implement a selector, add it to an EventModule in a DataCollectionPlan, then add the histograms to that EventModule's EventHistogramModule

class PASSelector : public Selector
{
    public:
        ~PASSelector() override {}
        //Keeps tight electrons and muons with pT greater than 40 and eta between -3 and 3 (muons) and -2.8 and 2.8 (electrons)
        void selectParticles(const InputModule* input, Event& event) override;
};

#endif