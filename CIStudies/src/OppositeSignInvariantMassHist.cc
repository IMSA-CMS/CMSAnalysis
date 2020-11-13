#include "CIAnalysis/CIStudies/interface/OppositeSignInvariantMassHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

double OppositeSignInvariantMassHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getGenSim().getGenParticles();
    auto genSimInv = genParticles.calculateOppositeSignInvariantMass();
    return genSimInv;
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getReco().getRecoCandidates();
    auto recoInv = recoParticles.calculateOppositeSignInvariantMass();
    return recoInv;
  }
}
