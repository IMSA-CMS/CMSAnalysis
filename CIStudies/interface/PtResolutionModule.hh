#ifndef PTRESOLUTIONMODULE_HH
#define PTRESOLUTIONMODULE_HH

#include "ResolutionModule.hh"

class MatchingPairCollection;

class PtResolutionModule : public ResolutionModule
{
public:
  PtResolutionModule(const MatchingModule& matchingModule, const int minPt = 50, const int maxPt = 1900, const int pTIntervalSize = 50);

protected:
  virtual void fillError(const MatchingPairCollection& matchingPairCollection) override;
};

#endif
