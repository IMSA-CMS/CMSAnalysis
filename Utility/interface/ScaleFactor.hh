#ifndef SCALEFACTOR_HH
#define SCALEFACTOR_HH

#include <string>

class EventInput; // Forward declaration

class ScaleFactor 
{
    
public:
    enum class SystematicType 
    {
        Nominal,
        Up,
        Down
    };
    ScaleFactor(std::string name) : name(name) {}
    virtual ~ScaleFactor() = default;
    virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const = 0; // Pure virtual function
    std::string getName() const { return name; }
private:
    std::string name;
    
     
};

#endif // SCALEFACTOR_HH
