#ifndef L1PREFIRINGWEIGHTRECIPE_HH
#define L1PREFIRINGWEIGHTRECIPE_HH

#include "CMSAnalysis/Modules/interface/EventScaleFactor.hh"

class L1PrefiringWeightRecipe : public EventScaleFactor
{
public:
    L1PrefiringWeightRecipe(std::string name, std::shared_ptr<ScaleFactorReader> reader);

    double getNominalWeight(const EventInput* input) const;
    double getUpWeight(const EventInput* input) const;
    double getDownWeight(const EventInput* input) const;

protected:
    virtual std::vector<std::string> getKey(const EventInput* input) const override;

private:
};

#endif