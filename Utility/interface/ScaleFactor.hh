#ifndef SCALEFACTOR_HH
#define SCALEFACTOR_HH

#include <memory>
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
    ScaleFactor(std::string iname, std::shared_ptr<ScaleFactorReader> ireader);
    virtual ~ScaleFactor() = default;
    virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const = 0; // Pure virtual function
    std::string getName() const { return name; }
private:
    std::string name;
    std::map<double, std::map<double, ScaleFactorSet>> scaleFactors;
     
};

#endif // SCALEFACTOR_HH
