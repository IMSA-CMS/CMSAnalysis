#ifndef MULTIYEARSCALEFACTOR_HH
#define MULTIYEARSCALEFACTOR_HH

#include "CMSAnalysis/Modules/interface/JSONScaleFactor.hh"
#include <map>
#include <string>

class EventInput;

class MultiYearScaleFactor : public ScaleFactor 
{
public:
	void addScaleFactor(std::string year, JSONScaleFactor scaleFactor);
    virtual double getScaleFactor(const EventInput* input) const override;
private:
    std::map<std::string, JSONScaleFactor> scaleFactors;
};

#endif // MULTIYEARSCALEFACTOR_HH
