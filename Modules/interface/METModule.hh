#ifndef METMODULE_HH
#define METMODULE_HH

#include <vector>

#include "ProductionModule.hh"

#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

namespace edm
{
  class EventBase;
}

class METModule : public ProductionModule
{
public:
  virtual bool process() override;

  double getMet() const {return met;}
  double getPhi() const {return phi;}

private:
  double met;
  double phi;

};

#endif
