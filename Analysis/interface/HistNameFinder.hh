#ifndef HISTNAMEFINDER_HH
#define HISTNAMEFINDER_HH

#include <string>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"

class HistNameFinder
{
public: //add systematic type
    virtual std::string getHistName(HistVariable histVariable, SystematicType type) const = 0;

};

#endif // HISTNAMEFINDER_HH