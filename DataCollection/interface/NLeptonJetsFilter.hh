#ifndef NLEPTONJETSFILTER_HH
#define NLEPTONJETSFILTER_HH

#include <memory>
#include "FilterModule.hh"

class NLeptonJetsFilter : public FilterModule
{
public:
  NLeptonJetsFilter();

protected:
  virtual std::string makeFilterString() override;
};

#endif
