#ifndef JSONSCALEFACTOR_HH
#define JSONSCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include "EventFilter/Utilities/interface/json.h"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <map>
#include <string>

class EventInput;

class JSONScaleFactor : public ScaleFactor 
{
public:
    JSONScaleFactor(std::string filename);
    virtual double getScaleFactor(const EventInput* input) const override;
    void printScaleFactors() const;  
protected:
    virtual void loadScaleFactors(Json::Value output) = 0;
    void addScaleFactor(double eta, double pt, double scaleFactors);
    void loadScaleFactorsFromFile(std::string filename);
    virtual ParticleCollection<Particle> getParticles(const EventInput* input) const = 0;
private:
    std::map<double, std::map<double, double>> scaleFactors;
};

#endif // JSONSCALEFACTOR_HH
