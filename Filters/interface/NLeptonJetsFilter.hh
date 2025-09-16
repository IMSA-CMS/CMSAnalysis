#ifndef NLEPTONJETSFILTER_HH
#define NLEPTONJETSFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"


class NLeptonJetsFilter : public Filter
{

public:
  virtual std::string getFilterString(const EventInput* input) const override;
};

#endif
