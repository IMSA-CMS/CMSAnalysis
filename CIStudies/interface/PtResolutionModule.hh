#ifndef PTRESOLUTIONMODULE_HH
#define PTRESOLUTIONMODULE_HH

#include <memory>
#include "ResolutionModule.hh"

class MatchingPairCollection;

class PtResolutionModule : public ResolutionModule
{
public:
  PtResolutionModule(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const std::shared_ptr<WeightingModule> weightingModule, const std::shared_ptr<LRWeightModule> lrWeightModule, const std::shared_ptr<MatchingModule> matchingModule, std::string bin = "pT", const int minPt = 50, const int maxPt = 1900, const int pTIntervalSize = 50);

protected:
  virtual void fillError(const MatchingPairCollection& matchingPairCollection) override;
};

#endif
