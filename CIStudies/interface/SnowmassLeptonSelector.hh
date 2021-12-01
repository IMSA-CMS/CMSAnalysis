#ifndef SNOWMASSLEPTONSELECTOR_HH
#define SNOWMASSLEPTONSELECTOR_HH

#include "Selector.hh"

class SnowmassLeptonSelector : public Selector
{
public:
    SnowmassLeptonSelector(double ptCut = 0);
    virtual bool checkParticle(const Particle &particle) const override;

private:
    const double ptCut;
};

#endif