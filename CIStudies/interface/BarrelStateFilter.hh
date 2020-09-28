#ifndef BARRELSTATEFILTER_HH
#define BARRELSTATEFILTER_HH

#include "FilterModule.hh"

class MatchingModule;

class BarrelStateFilter : public FilterModule
{
public:
  BarrelStateFilter(const MatchingModule& matchingModule);

protected:
  virtual std::string makeFilterString(const edm::EventBase& event) override;

private:
  const MatchingModule& matchMod;
};

#endif
