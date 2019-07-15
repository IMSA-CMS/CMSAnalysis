#ifndef GENSIMIDENTIFICATIONMODULE_HH
#define GENSIMIDENTIFICATIONMODULE_HH

#include <vector>

#include "ProductionModule.h"

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
  virtual void process(const edm::EventBase& event) override;
  bool isParticle(const reco::GenParticle& p) const;

  const std::vector<const reco::GenParticle*>& getGenParticles() const
  {return genParticles;}

private:
  std::vector<const reco::GenParticle*> genParticles;
  double ptCut;
  bool isElectronIfTrue;
};

#endif


