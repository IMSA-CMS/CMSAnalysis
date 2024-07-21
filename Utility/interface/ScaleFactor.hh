#ifndef JSONSCALEFACTOR_HH
#define JSONSCALEFACTOR_HH

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class EventInput;
class JSONScaleFactor: public ScaleFactor 
{
public:
    JSONScaleFactor(std::string filename);
    virtual double getScaleFactor(const EventInput* input) const override;
    void printScaleFactors() const;  
private:
    std::map<double, std::map<double, double>> scaleFactors;
};

#endif // SCALEFACTOR_HH