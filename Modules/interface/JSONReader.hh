#ifndef JSONREADER_HH
#define JSONREADER_HH

#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"
#include "EventFilter/Utilities/interface/json.h"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <map>
#include <string>

class EventInput;

class JSONReader : public ScaleFactorReader
{
public:
    virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const override;
    void printScaleFactors() const;  
    JSONReader(std::string iname) : ScaleFactor(iname) {}
    struct ScaleFactorSet
    {
        double nominal;
        double systUp;
        double systDown;
        ScaleFactorSet(double nominal = 1.0, double systUp = 1.0, double systDown = 1.0) : nominal(nominal), systUp(systUp), systDown(systDown) {}
        ScaleFactorSet& operator*= (const ScaleFactorSet& rhs)
        {
            nominal *= rhs.nominal;
            systUp *= rhs.systUp;
            systDown *= rhs.systDown;
            return *this;
        }
    };
protected:
    virtual void loadScaleFactors(Json::Value output) = 0;
    void addScaleFactor(double eta, double pt, ScaleFactorSet scaleFactor);
    ScaleFactorSet& getScaleFactorSet(double eta, double pt);
    void loadScaleFactorsFromFile(std::string filename);
    virtual ParticleCollection<Particle> getParticles(const EventInput* input) const = 0;
   
private:
    std::map<double, std::map<double, ScaleFactorSet>> scaleFactors;
    SystematicType systematicType;

    
};

#endif // JSONSCALEFACTOR_HH
//add name to constructor and Systematic Type to getScaleFactor