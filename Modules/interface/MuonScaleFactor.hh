#ifndef MUONSCALEFACTOR_HH
#define MUONSCALEFACTOR_HH

#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>
#include <memory>
#include <map>
#include <string>

class EventInput;

class MuonScaleFactor : public JSONScaleFactor 
{
public:
    MuonScaleFactor(std::string filename);
protected:
    virtual void loadScaleFactors(Json::Value output) override;
    virtual ParticleCollection<Particle> getParticles(const EventInput* input) const override;

};

#endif // MUONSCALEFACTOR_HH
