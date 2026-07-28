#ifndef KANSASCUTS_HH
#define KANSASCUTS_HH
#include <vector>
#include <string>

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

class Particle;
class Lepton;

class KansasCuts : public Selector
{
    public:
        ~KansasCuts() {}
        virtual void selectParticles(const EventInput* input, Event& event) const override;

};

#endif