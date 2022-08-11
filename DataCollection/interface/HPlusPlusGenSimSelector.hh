#ifndef HPLUSPLUSGENSIMSELECTOR_HH
#define HPLUSPLUSGENSIMSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/Selector.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

class InputModule;

class HPlusPlusGenSimSelector : public Selector
{
    public:
        void selectParticles(const InputModule* input, Event& event) override;
};

#endif