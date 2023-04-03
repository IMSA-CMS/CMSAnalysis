#ifndef MATCHINGPAIR_HH
#define MATCHINGPAIR_HH

#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"

// MatchingPair contains the gen particle with its respective reco particle
// get functions calculate delta/error values between the two particles
class MatchingPair
{
public:
  MatchingPair(GenSimParticle bestGenParticle, Particle bestRecoParticle);
  double getDeltaPhi() const; //getter methods
  double getDeltaR() const;
  double getPtError() const;
  double getEtaError() const;

  GenSimParticle getGenParticle() const {return genParticle;}
  Particle getRecoParticle() const {return recoParticle;}

private:
  GenSimParticle genParticle;
  Particle recoParticle;

  double calculateError(double exp, double theo) const;
};

#endif
