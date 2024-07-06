
#ifndef GENSIMSTUDYMODULE_HH
#define GENSIMSTUDYMODULE_HH
#include <vector>

#include "CMSAnalysis/Modules/interface/ProductionModule.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

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

  virtual std::string getName() override {return "GenSimStudyModule";}

private:
    int isPhoton;
};

#endif
