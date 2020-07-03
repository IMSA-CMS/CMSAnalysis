#ifndef LRWEIGHTMODULE_HH
#define LRWEIGHTMODULE_HH

#include <string>
#include <utility>
#include <vector>

#include "ProductionModule.hh"

#include "FileParams.hh"
#include "Pythia8/Pythia.h"

namespace reco
{
  class GenParticle;
  class Candidate;
}

class GenEventInfoProduct;

class LRWeightModule : public ProductionModule

{
public:

  LRWeightModule() {pythia.init();}

  virtual bool process(const edm::EventBase& event) override;
  double getLRWeight() const {return lrWeight;}
  double getRLWeight() const {return rlWeight;}

private:

  double lrWeight;
  double rlWeight;

  // Mutable because Pythia isn't very careful with const correctness
  mutable Pythia8::Pythia pythia;

  std::pair<double, double> calculateWeights(const GenEventInfoProduct& genEventInfoProduct, 
					     const std::vector<reco::GenParticle>& gen, double lam, 
					     int inter) const;

  const reco::Candidate* getBosonMother(const reco::GenParticle& p) const;
  const reco::Candidate* getLeptonMother(const reco::GenParticle& p, bool second) const;
};

#endif
