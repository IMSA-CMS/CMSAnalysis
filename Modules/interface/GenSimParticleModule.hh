#ifndef GENSIMPARTICLEMODULE_HH
#define GENSIMPARTICLEMODULE_HH

#include <vector>

#include "ProductionModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

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
  virtual bool process() override;

  const ParticleCollection<GenSimParticle>& getGenParticles() const 
  {return genParticles;}

private:
  ParticleCollection<GenSimParticle> genParticles;
  int targetPdgId;
};

#endif


