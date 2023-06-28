#ifndef HIGGSSELECTOR_HH
#define HIGGSSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"

class Particle;
class Lepton;

class HiggsSelector : public Selector
{
    public:
        ~HiggsSelector() {}
        virtual void selectParticles(const InputModule* input, Event& event) const override;
};

#endif