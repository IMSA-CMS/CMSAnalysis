#ifndef SCALEFACTOR_HH
#define SCALEFACTOR_HH

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <iostream>

class EventInput;
class ScaleFactor 
{

public:
    virtual ~ScaleFactor() {}
    virtual double getScaleFactor(const EventInput* input) const = 0;
};

#endif // SCALEFACTOR_HH
