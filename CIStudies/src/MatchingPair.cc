#include "CIAnalysis/CIStudies/interface/MatchingPair.hh"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"


MatchingPair::MatchingPair(const reco::GenParticle* bestGenParticle, const reco::RecoCandidate* bestRecoParticle) :
  genParticle(bestGenParticle),
  recoParticle(bestRecoParticle)
{
}

double MatchingPair::getDeltaPhi() const
{
  double recoPhi = recoParticle->phi();
  double genPhi = genParticle->phi();
  const double pi = 3.1415926535897932384;

  double actualPhiDif = recoPhi - genPhi;
  double phiDif = std::abs(actualPhiDif);

  if (phiDif > pi)
    {
      phiDif = (2 * pi) - phiDif;
    }

  return phiDif; 
}


double MatchingPair::getDeltaR() const
{
  double recoEta = recoParticle->eta();
  double genEta = genParticle->eta();
  double etaDif = recoEta - genEta;

  double phiDif = getDeltaPhi();

  return std::sqrt((etaDif * etaDif) + (phiDif * phiDif));
}

double MatchingPair::getPtError() const
{
  return calculateError(recoParticle->pt(), genParticle->pt());
}

double MatchingPair::getEtaError() const
{
  return calculateError(recoParticle->eta(), genParticle->eta());
}

double MatchingPair::calculateError(double exp, double theo) const
{
  return ((exp - theo) / theo); 
}
