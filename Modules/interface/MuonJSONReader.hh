#ifndef MUONJSONREADER_HH
#define MUONJSONREADER_HH

#include "CMSAnalysis/Modules/interface/JSONReader.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <iostream>
#include <memory>
#include <map>
#include <string>

class EventInput;

class MuonJSONReader : public JSONReader 
{
public:
    MuonJSONReader(std::string filename);
protected:
    virtual std::map<std::string, ScaleFactor::ScaleFactorSet>  loadScaleFactors(jsoncollector::Json::Value output) override;
    // virtual std::string getKey(Lepton lepton) override;


};

#endif // MUONSCALEFACTOR_HH
