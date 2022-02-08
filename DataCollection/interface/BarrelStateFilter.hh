#ifndef BARRELSTATEFILTER_HH
#define BARRELSTATEFILTER_HH

#include <memory>
#include "FilterModule.hh"

class MatchingModule;

class BarrelStateFilter : public FilterModule
{
public:
  BarrelStateFilter(const std::shared_ptr<MatchingModule> matchingModule);

protected:
  virtual std::string makeFilterString() override;

private:
  const std::shared_ptr<MatchingModule> matchMod;
};

#endif
