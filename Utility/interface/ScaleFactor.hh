#ifndef SCALEFACTOR_HH
#define SCALEFACTOR_HH

class EventInput; // Forward declaration

class ScaleFactor 
{
public:
    virtual ~ScaleFactor() = default;
    virtual double getScaleFactor(const EventInput* input) const = 0; // Pure virtual function
};

#endif // SCALEFACTOR_HH
