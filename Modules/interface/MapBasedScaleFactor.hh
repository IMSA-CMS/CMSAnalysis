#ifndef MAPBASEDSCALEFACTOR_HH
#define MAPBASEDSCALEFACTOR_HH
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class MapBasedScaleFactor : public ScaleFactor
{
    
public:

    

    
    MapBasedScaleFactor(std::string iname, std::shared_ptr<ScaleFactorReader> ireader);
    virtual ~MapBasedScaleFactor() = default;
    virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const;

 protected:
     virtual std::vector<std::string> getKey(const EventInput* input) const = 0;
     //std::map<std::string, ScaleFactorSet> & getScaleFactorMap() { return scaleFactors; }
     const std::map<std::string, ScaleFactorSet> & getScaleFactorMap() const { return scaleFactors; }



private:
    std::shared_ptr<ScaleFactorReader> reader;
    // The scale factors are stored in a map with the key being the year and the value
    std::map<std::string, ScaleFactorSet> scaleFactors;
};

#endif // SCALEFACTOR_HH
