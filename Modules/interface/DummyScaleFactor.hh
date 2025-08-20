#ifndef DUMMYSCALEFACTOR_HH
#define DUMMYSCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include <map>
#include <string>

class EventInput;

class DummyScaleFactor : public ScaleFactor
{
public:
    DummyScaleFactor();

protected:
    virtual void loadScaleFactors(Json::Value output) override;
    virtual ParticleCollection<Particle> getParticles(const EventInput* input) const override;

};

#endif // DUMMYSCALEFACTOR_HH
