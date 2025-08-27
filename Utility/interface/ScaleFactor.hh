#ifndef SCALEFACTOR_HH
#define SCALEFACTOR_HH
#include <vector>
#include <memory>
#include <string>
#include <map>

class ScaleFactorReader;
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
    struct ScaleFactorSet
    {
        double nominal;
        double systUp;
        double systDown;
        ScaleFactorSet(double nominal = 1.0, double systUp = 1.0, double systDown = 1.0) : nominal(nominal), systUp(systUp), systDown(systDown) {}
        ScaleFactorSet& operator*= (const ScaleFactorSet& rhs)
        {
            nominal *= rhs.nominal;
            systUp *= rhs.systUp;
            systDown *= rhs.systDown;
            return *this;
        }

        double getSystematic(SystematicType type) const
        {
            if (type == SystematicType::Nominal)
            {
                return nominal;
            }
            else if (type == SystematicType::Down)
            {
                return systDown;
            }
            else
            {
                return systUp;
            }
        }
    };

    
    ScaleFactor(std::string iname, std::shared_ptr<ScaleFactorReader> ireader);
    virtual ~ScaleFactor() = default;
    virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const;
    std::string getName() const { return name; }

 protected:
     virtual std::vector<std::string> getKey(const EventInput* input) const = 0;

private:
    std::string name;
    std::shared_ptr<ScaleFactorReader> reader;
    // The scale factors are stored in a map with the key being the year and the value
    std::map<std::string, ScaleFactorSet> scaleFactors;


     
};

#endif // SCALEFACTOR_HH
