#ifndef LRWEIGHTMODULE_HH
#define LRWEIGHTMODULE_HH

#include <string>
#include <utility>
#include <vector>

#include "ProductionModule.hh"

#include "FileParams.hh"

namespace reco
{
  class GenParticle;
  class Candidate;
}

class GenEventInfoProduct;

class LRWeightModule : public ProductionModule

{
public:

  LRWeightModule();

  virtual bool process(const edm::EventBase& event) override;
  double getLRWeight() const {return lrWeight;}
  double getRLWeight() const {return rlWeight;}

private:

  double lrWeight;
  double rlWeight;

  double findAf(int idAbs) const;
  double findVf(int idAbs) const;
  double findEf(int idAbs) const;
  double findSin2thetaW() const {return 0.231;}
  double findCos2thetaW() const {return 0.769;}

  std::pair<double, double> calculateWeights(const GenEventInfoProduct& genEventInfoProduct, 
					     const std::vector<reco::GenParticle>& gen, double lam, 
					     int inter) const;

  const reco::Candidate* getBosonMother(const reco::GenParticle& p) const;
  const reco::Candidate* getLeptonMother(const reco::GenParticle& p, bool second) const;
};

#endif
