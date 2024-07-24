#ifndef PTRESOLUTIONMODULE_HH
#define PTRESOLUTIONMODULE_HH

#include <memory>
#include "ResolutionModule.hh"

class MatchingPairCollection;

class PtResolutionModule : public ResolutionModule
{
public:
  PtResolutionModule(const std::shared_ptr<MatchingModule> matchingModule, std::string bin = "pT", const int minPt = 50, const int maxPt = 1900, const int pTIntervalSize = 50);

  virtual std::string getName() override {return "PtResolutionModule";}

protected:
  virtual void fillError(const MatchingPairCollection& matchingPairCollection) override;
};

#endif
