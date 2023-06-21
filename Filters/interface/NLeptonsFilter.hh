#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"

class NLeptonsFilter : public Filter
{
public:
  NLeptonsFilter();

protected:
  virtual std::string getFilterString(const InputModule* inputMod) const override;
};

#endif
