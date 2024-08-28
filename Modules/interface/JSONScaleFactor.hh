#ifndef JSONSCALEFACTOR_HH
#define JSONSCALEFACTOR_HH

#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include <map>
#include <string>

class EventInput;

class JSONScaleFactor : public ScaleFactor 
{
public:
    JSONScaleFactor(std::string filename);
    virtual double getScaleFactor(const EventInput* input) const override;
    void printScaleFactors() const;  
private:
    std::map<double, std::map<double, double>> scaleFactors;
};

#endif // JSONSCALEFACTOR_HH
