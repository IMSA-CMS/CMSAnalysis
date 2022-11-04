#ifndef SELECTOR_HH
#define SELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/Event.hh"

class Particle;
class Lepton;

class Selector
{
    public:
        virtual ~Selector() {}
        virtual void selectParticles(const InputModule* input, Event& event) = 0;
};

#endif