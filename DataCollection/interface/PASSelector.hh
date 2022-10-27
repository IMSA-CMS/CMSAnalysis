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

class PASSelector : public Selector
{
    public:
        ~PASSelector() override {}
        void selectParticles(const InputModule* input, Event& event) override;
};

#endif