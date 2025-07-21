#ifndef HIGGSZVETOFILTER_HH
#define HIGGSZVETOFILTER_HH

#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/Filter.hh"

class HiggsZVetoFilter : public Filter
{
  public:
    virtual std::string getFilterString(const EventInput *input) const override;
};

#endif