#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "FilterModule.hh"

//class RecoIdentificationModule;

class NLeptonsFilter : public FilterModule
{
protected:
  virtual std::string makeFilterString() override;
};

#endif
