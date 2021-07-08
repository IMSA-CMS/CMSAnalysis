#ifndef GENSIMIDENTIFICATIONMODULE_HH
#define GENSIMIDENTIFICATIONMODULE_HH

#include <vector>

#include "ProductionModule.hh"
#include "ParticleCollection.hh"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Handle.h"

namespace edm
{
  class EventBase;
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
  std::vector<reco::GenParticle> getPhotons() const;
private:
  // checks if the gen particles originates from the hard scatter process
  bool isParticle(Particle p) const;
  

  ParticleCollection genParticles;
  ParticleCollection photonList;
  int targetPdgId;
  const bool ignoreRadiation;
  int photonNum;
  edm::Handle<std::vector<reco::GenParticle>> genParticlesHandle;
};

#endif


