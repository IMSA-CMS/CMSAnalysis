#ifndef SELECTOR_HH
#define SELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

class Particle;
class Lepton;

class Selector
{
    public:
        virtual ~Selector() {}
        virtual std::vector<Particle> selectParticles(const InputModule* input) const = 0;
};

#endif