#ifndef MATCHINGMODULE_HH
#define MATCHINGMODULE_HH

#include "ProductionModule.hh"

#include <vector>

class MatchingModule : public ProductionModule
{
public:
  MatchingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModuke& recoModule);
  virtual void process(const edm::EventBase& event) override;

struct MatchedList
  {
    const reco::GenParticle* bestGenParticle;
    const reco::RecoCandidate* bestRecoParticle;
    double deltaR;
    double phiError;
    double etaError;
    double ptError;
  };

private:
  const GenSimIdentificationModule& genSim;
  const RecoIdentificationModule& reco;
};

#endif
