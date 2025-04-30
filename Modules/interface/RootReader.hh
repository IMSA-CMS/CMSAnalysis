#ifndef ROOTREADER_HH
#define ROOTREADER_HH

#include "CMSAnalysis/Modules/interface/ScaleFactorReader.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <map>
#include <string>

class EventInput;
class TH1;

class RootReader : public ScaleFactorReader 
{
public:

    RootReader(std::string filename, std::string histname);
    virtual double getScaleFactor(const EventInput* input) const override;
    void printScaleFactors() const;  

private:
	TH1* histogram;
};

#endif // JSONSCALEFACTOR_HH
