#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "FilterModule.hh"

class NLeptonsFilter : public FilterModule
{
public:
  NLeptonsFilter();

protected:
  virtual std::string makeFilterString() override;
};

#endif
