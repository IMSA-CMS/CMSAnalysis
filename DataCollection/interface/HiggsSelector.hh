#ifndef HIGGSSELECTOR_HH
#define HIGGSSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

class Particle;
class Lepton;

class HiggsSelector : public Selector
{
    public:
        ~HiggsSelector() override {}
        std::vector<Particle> selectParticles(const InputModule* input) const override;
};

#endif