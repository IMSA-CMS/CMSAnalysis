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

// GenSimIdentificationModule creates a Particle Collection
// of gen sim particles that will be used in MatchingModule 
class GenSimIdentificationModule : public ProductionModule
{
public:
  GenSimIdentificationModule(int itargetPdgId = 0 , bool ignoreRadiation = false);
  virtual bool process(const edm::EventBase& event) override;

  const ParticleCollection& getGenParticles() const
  {return genParticles;}

private:
  // checks if the gen particles originates from the hard scatter process
  bool isParticle(Particle p) const;

  ParticleCollection genParticles;
  int targetPdgId;
  const bool ignoreRadiation;
};

#endif


