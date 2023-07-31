#ifndef BARRELSTATEFILTER_HH
#define BARRELSTATEFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"

class MatchingModule;

class BarrelStateFilter : public Filter
{
public:
  BarrelStateFilter(const std::shared_ptr<MatchingModule> matchingModule);

protected:
  virtual std::string getFilterString(const EventInput* inputMod) const override; // Reformate and Validates the filter string

private:
  const std::shared_ptr<MatchingModule> matchMod;
};

#endif
