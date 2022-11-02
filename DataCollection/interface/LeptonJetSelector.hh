#ifndef LEPTONJETSELECTOR_HH
#define LEPTONJETSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/DataCollection/interface/Selector.hh"

class LeptonJetSelector : public Selector
{
    public:
        void selectParticles(const InputModule* input, Event& event) override;
};

#endif
