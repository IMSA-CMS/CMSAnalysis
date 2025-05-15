#ifndef HISTNAMEFINDER_HH
#define HISTNAMEFINDER_HH

#include <string>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"


class HistNameFinder
{
public: //add systematic type
    virtual std::string getHistName(HistVariable histVariable, ScaleFactor::SystematicType type, std::string systematicName) const = 0;
    virtual ~HistNameFinder() = default;
};

#endif // HISTNAMEFINDER_HH