#ifndef RECOIDENTIFICATIONMODULE_HH
#define RECOIDENTIFICATIONMODULE_HH

#include <vector>

#include "ProductionModule.hh"
#include "ParticleCollection.hh"

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

  const ParticleCollection& getRecoCandidates() const
  {return recoCandidates;}

private:
  ParticleCollection recoCandidates;
  const double ptCut;
};

#endif
