#ifndef ELECTRONSCALEFACTOR_HH
#define ELECTRONSCALEFACTOR_HH

#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include <map>
#include <string>

class EventInput;

class ElectronScaleFactor : public JSONScaleFactor 
{
public:
    ElectronScaleFactor(std::string filename);

protected:
    virtual void loadScaleFactors(jsoncollector::Json::Value output) override;
    virtual ParticleCollection<Particle> getParticles(const EventInput* input) const override;

private:
    void loadScaleFactors(jsoncollector::Json::Value output, SystematicType systematicType);

};

#endif // ELECTRONSCALEFACTOR_HH
