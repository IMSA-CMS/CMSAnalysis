#ifndef MATCHINGPAIR_HH
#define MATCHINGPAIR_HH

namespace reco
{
  class GenParticle;
  class RecoCandidate;
}

// MatchingPair contains the gen particle with its respective reco particle
// get functions calculate delta/error values between the two particles
class MatchingPair
{
public:
  MatchingPair(const reco::GenParticle* bestGenParticle, const reco::RecoCandidate* bestRecoParticle);
  double getDeltaPhi() const;
  double getDeltaR() const;
  double getPtError() const;
  double getEtaError() const;

  const reco::GenParticle* getGenParticle() const {return genParticle;}
  const reco::RecoCandidate* getRecoParticle() const { return recoParticle;}

private:
  const reco::GenParticle* genParticle;
  const reco::RecoCandidate* recoParticle;

  double calculateError(double exp, double theo) const;
};

#endif
