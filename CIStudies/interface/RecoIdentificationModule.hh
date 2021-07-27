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

  virtual bool process() override;

  const ParticleCollection& getRecoCandidates() const
  {return recoCandidates;}

  ParticleCollection getRecoCandidates(Particle::LeptonType leptonType) const;      // Returns the Reco Candidates for a certain lepton type

private:
  ParticleCollection recoCandidates;
  const double ptCut;
};

#endif
