#ifndef MATCHINGPAIR_HH
#define MATCHINGPAIR_HH

#include "CIAnalysis/CIStudies/interface/Particle.hh"

// MatchingPair contains the gen particle with its respective reco particle
// get functions calculate delta/error values between the two particles
class MatchingPair
{
public:
  MatchingPair(Particle bestGenParticle, Particle bestRecoParticle);
  double getDeltaPhi() const;
  double getDeltaR() const;
  double getPtError() const;
  double getEtaError() const;

  Particle getGenParticle() const {return genParticle;}
  Particle getRecoParticle() const {return recoParticle;}

  //void setGenParticle(Particle inputParticle) {genParticle = inputParticle;}
  //void setRecoParticle(Particle inputParticle) {recoParticle = inputParticle;}

private:
  Particle genParticle;
  Particle recoParticle;

  double calculateError(double exp, double theo) const;
};

#endif
