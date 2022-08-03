#ifndef DARKPHOTONGENSIMSELECTOR_HH
#define DARKPHOTONGENSIMSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

class InputModule;

class DarkPhotonGenSimSelector : public Selector
{
    public:
        std::vector<Particle> selectParticles(const InputModule* input) const override;
};

#endif