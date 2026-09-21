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

        ScaleFactor(std::string iname, bool hasUncertainty = true) : name(std::move(iname)), uncertainty(hasUncertainty) {}
        virtual ~ScaleFactor() = default;

        virtual double getScaleFactor(const EventInput* input,
                                    SystematicType type = SystematicType::Nominal) const = 0;

        std::string getName() const { return name; }
        bool hasUncertainty() const { return uncertainty; }

    private:
        std::string name;
        bool uncertainty;
};

#endif // SCALEFACTOR_HH
