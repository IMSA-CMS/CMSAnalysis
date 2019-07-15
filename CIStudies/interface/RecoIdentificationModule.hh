#ifndef RECOIDENTIFICATIONMODULE_HH
#define RECOIDENTIFICATIONMODULE_HH

#include <vector>

#include "ProductionModule.h"

namespace edm
{
  class EventBase;
}

namespace reco
{
  class RecoCandidate;
}

class RecoIdentificationModule : public ProductionModule
{
public:
  virtual void process(const edm::EventBase& event) override;

  const std::vector<const reco::RecoCandidate*>& getRecoCandidates() const
  {return recoCandidates;}

private:
  std::vector<const reco::RecoCandidate*> recoCandidates;
  double ptCut;
  bool isElectronIfTrue;
};

#endif
