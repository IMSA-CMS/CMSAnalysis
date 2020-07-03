#ifndef MASSRESOLUTIONMODULE_HH
#define MASSRESOLUTIONMODULE_HH

#include "ResolutionModule.hh"

class MatchingPairCollection;

// MassResolutionModule plots the error between gen sim and reco
// invariant mass over differet mass ranges
class MassResolutionModule : public ResolutionModule
{
public:
  MassResolutionModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, const WeightingModule& weightingModule, const MatchingModule& matchingModule, std::string bin = "mass", const int minMass = 300, const int maxMass = 3200, const int massIntervalSize = 100);

protected:
  //calculates mass error and fills the histogram with the respective mass bin
  virtual void fillError(const MatchingPairCollection& matchingPairCollection) override; 
};

#endif
