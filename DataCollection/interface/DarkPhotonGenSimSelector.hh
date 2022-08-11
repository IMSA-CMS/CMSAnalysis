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
        void selectParticles(const InputModule* input, Event& event) override;
    private:
        std::vector<Particle> checkJet(GenSimParticle part) const;
};

#endif