#ifndef MUONSELECTOR_HH
#define MUONSELECTOR_HH
#include <vector>
#include <string>

#include "CMSAnalysis/Filters/interface/Selector.hh"


class MuonSelector : public Selector
{
    public:
        ~MuonSelector() {}
        virtual void selectParticles(const EventInput* input, Event& event) const override;
};

#endif