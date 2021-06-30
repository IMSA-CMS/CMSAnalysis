#ifndef GENSIMPARTICLEMODULE_HH
#define GENSIMPARTICLEMODULE_HH

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

class GenSimParticleModule : public ProductionModule
{
public:
  GenSimParticleModule(int itargetPdgId = 0);
  virtual bool process(const edm::EventBase& event) override;

  const ParticleCollection& getGenParticles() const
  {return genParticles;}

private:
  ParticleCollection genParticles;
  int targetPdgId;
};

#endif


