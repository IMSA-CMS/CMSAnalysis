#ifndef ELECTRONJSONREADER_HH
#define ELECTRONJSONREADER_HH

#include "CMSAnalysis/Modules/interface/JSONReader.hh"
#include <map>
#include <string>

class EventInput;

class ElectronJSONReader : public JSONReader
{
public:
    ElectronReader(std::string filename);

protected:
    virtual void loadScaleFactors(Json::Value output) override;
    virtual ParticleCollection<Particle> getParticles(const EventInput* input) const override;

private:
    void loadScaleFactors(Json::Value output, SystematicType systematicType);

};

#endif // ELECTRONSCALEFACTOR_HH
