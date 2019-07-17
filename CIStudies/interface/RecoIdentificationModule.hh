#ifndef RECOIDENTIFICATIONMODULE_HH
#define RECOIDENTIFICATIONMODULE_HH

#include <vector>

#include "ProductionModule.hh"

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
  RecoIdentificationModule(double iptCut = 50);

  virtual bool process(const edm::EventBase& event) override;

  const std::vector<const reco::RecoCandidate*>& getRecoCandidates() const
  {return recoCandidates;}

private:
  std::vector<const reco::RecoCandidate*> recoCandidates;
  const double ptCut;
};

#endif
