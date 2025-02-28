#ifndef ROOTSCALEFACTOR_HH
#define ROOTSCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include <map>
#include <string>

class EventInput;
class TH1;

class RootScaleFactor : public ScaleFactor 
{
public:

    RootScaleFactor(std::string filename, std::string histname);
    virtual double getScaleFactor(const EventInput* input) const override;
    void printScaleFactors() const;  

private:
	TH1* histogram;
};

#endif // JSONSCALEFACTOR_HH
