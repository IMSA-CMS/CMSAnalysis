#ifndef ROCHESTERSCALEFACTOR_HH
#define ROCHESTERSCALEFACTOR_HH
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "TRandom3.h"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class RoccoR;

class RochesterScaleFactor : public ScaleFactor
{
    
public:
    RochesterScaleFactor(std::string iname, std::string correctionFile);
    virtual ~RochesterScaleFactor();
    virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const;

private:
    std::unique_ptr<RoccoR> roccoR_;
    mutable TRandom3 rng;
};

#endif // SCALEFACTOR_HH

// write source file for this class and then variables are in the nanoAOD event file, so we can get them from there. We can also add more variables in the future if we want to. We can also add more systematic types if we want to.
