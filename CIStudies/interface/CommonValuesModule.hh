#ifndef COMMONVALUESMODULE_HH
#define COMMONVALUESMODULE_HH

#include "ProductionModule.hh"
#include "MatchingModule.hh"

#include "TLorentzVector.h"

#include <vector>

namespace reco
{
  class Candidate;
}

class CommonValuesModule : public ProductionModule
{
public:
  CommonValuesModule(const MatchingModule& matchingModule);
  virtual bool process(const edm::EventBase& event) override;

  struct GenSimAndRecoValues
  {
    double genSim;
    double reco;
  };

  GenSimAndRecoValues getInvariantMass() const {return invariantMass;}
  GenSimAndRecoValues getCollinsSoper() const {return collinsSoper;}

private:
  double findInvariantMass(const reco::Candidate* particle, const reco::Candidate* antiparticle) const;

  double findCollinsSoper(const reco::Candidate* particle, const reco::Candidate* antiparticle) const;

  bool lowEtaFlip(const reco::Candidate* particle, const reco::Candidate* antiparticle) const;

  double calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar) const;

  GenSimAndRecoValues invariantMass;
  GenSimAndRecoValues collinsSoper;
  
  const MatchingModule& matching;
};

#endif
