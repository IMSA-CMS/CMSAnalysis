#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"

class NLeptonsFilter : public Filter
{
public:
  NLeptonsFilter();

protected:
  virtual std::string getFilterString(const EventInput* inputMod) const override;
};

#endif
