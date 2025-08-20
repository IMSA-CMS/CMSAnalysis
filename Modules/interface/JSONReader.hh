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
  
 
protected:
    virtual std::map<std::string, ScaleFactor::ScaleFactorSet> loadScaleFactors(Json::Value output) = 0;
    std::map<std::string, ScaleFactor::ScaleFactorSet> loadScaleFactorsFromFile(std::string filename);

private:
    std::string preprocessJSON(const std::string &filename); 
};

#endif // JSONSCALEFACTOR_HH
//add name to constructor and Systematic Type to getScaleFactor