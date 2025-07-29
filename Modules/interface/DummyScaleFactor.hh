#ifndef DUMMYSCALEFACTOR_HH
#define DUMMYSCALEFACTOR_HH

#include "CMSAnalysis/Modules/interface/ElectronScaleFactor.hh"
#include <map>
#include <string>

class EventInput;

class DummyScaleFactor : public ElectronScaleFactor 
{
public:
    DummyScaleFactor();

protected:
    virtual void loadScaleFactors(jsoncollector::Json::Value output) override;
    virtual ParticleCollection<Particle> getParticles(const EventInput* input) const override;

};

#endif // DUMMYSCALEFACTOR_HH
