#ifndef ELECTRONJSONREADER_HH
#define ELECTRONJSONREADER_HH

#include "CMSAnalysis/Modules/interface/JSONReader.hh"
#include <map>
#include <string>

class EventInput;

class ElectronJSONReader : public JSONReader
{
public:
    ElectronJSONReader(std::string filename);

protected:
    virtual std::map<std::string, ScaleFactor::ScaleFactorSet> loadScaleFactors(jsoncollector::Json::Value output) override;

private:
void loadScaleFactors(jsoncollector::Json::Value output, ScaleFactor::SystematicType systematicType, std::map<std::string, ScaleFactor::ScaleFactorSet>& scaleFactorMap);

};

#endif // ELECTRONSCALEFACTOR_HH
