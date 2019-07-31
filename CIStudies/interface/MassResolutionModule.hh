#ifndef MASSRESOLUTIONMODULE_HH
#define MASSRESOLUTIONMODULE_HH

#include "ResolutionModule.hh"

class MatchingPairCollection;

class MassResolutionModule : public ResolutionModule
{
public:
  MassResolutionModule(const MatchingModule& matchingModule, const int minMass = 300, const int maxMass = 3200, const int massIntervalSize = 100);

protected:
  virtual void fillError(const MatchingPairCollection& matchingPairCollection) override;
};

#endif
