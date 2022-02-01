#include "CIAnalysis/CIStudies/interface/MatchingPair.hh"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "CIAnalysis/CIStudies/interface/Particle.hh"


MatchingPair::MatchingPair(Particle bestGenParticle, Particle bestRecoParticle) :
  genParticle(bestGenParticle),
  recoParticle(bestRecoParticle)
{
}

double MatchingPair::getDeltaPhi() const
{
  double recoPhi = recoParticle.getPhi();
  double genPhi = genParticle.getPhi();
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
  double recoEta = recoParticle.getEta();
  double genEta = genParticle.getEta();
  double etaDif = recoEta - genEta;

  double phiDif = getDeltaPhi();

  return std::sqrt((etaDif * etaDif) + (phiDif * phiDif));
}

double MatchingPair::getPtError() const
{
  return calculateError(recoParticle.getPt(), genParticle.getPt());
}

double MatchingPair::getEtaError() const
{
  return calculateError(recoParticle.getEta(), genParticle.getEta());
}

double MatchingPair::calculateError(double exp, double theo) const
{
  return ((exp - theo) / theo); 
}
