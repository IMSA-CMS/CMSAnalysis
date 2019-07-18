#ifndef GENSIMIDENTIFICATIONMODULE_HH
#define GENSIMIDENTIFICATIONMODULE_HH

#include <vector>

#include "ProductionModule.hh"
#include "ParticleCollection.hh"

namespace edm
{
  class EventBase;
}

namespace reco
{
  class GenParticle;
}

class GenSimIdentificationModule : public ProductionModule
{
public:
  virtual bool process(const edm::EventBase& event) override;

  const ParticleCollection& getGenParticles() const
  {return genParticles;}

private:
  bool isParticle(const reco::GenParticle& p) const;

  ParticleCollection genParticles;
  double ptCut;
  bool isElectronIfTrue;
};

#endif


