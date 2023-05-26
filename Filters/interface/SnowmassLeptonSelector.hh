/*#ifndef SNOWMASSLEPTONSELECTOR_HH
#define SNOWMASSLEPTONSELECTOR_HH

#include "Selector.hh"
#include "Lepton.hh"

class SnowmassLeptonSelector : public Selector
{
public:
    SnowmassLeptonSelector(double ptCut = 0, double isoCut = 100);
    virtual bool checkParticle(const Lepton &particle) const override;

private:
    const double ptCut;
    const double isoCut;
};

#endif*/