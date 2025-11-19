#ifndef HISTNAMEFINDER_HH
#define HISTNAMEFINDER_HH

#include <string>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"


class HistNameFinder
{
public: //add systematic type
    virtual std::string getHistName(const HistVariable& histVariable) const = 0;
    virtual ~HistNameFinder() = default;
};

#endif // HISTNAMEFINDER_HH