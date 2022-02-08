
#ifndef GENSIMSTUDYMODULE_HH
#define GENSIMSTUDYMODULE_HH
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


class GenSimStudyModule : public ProductionModule
{
public:
  GenSimStudyModule();
  virtual bool process() override;
private:
    int isPhoton;
};

#endif
