#include "CIAnalysis/CIStudies/interface/SameSignInvariantMassHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

SameSignInvariantMassHist::SameSignInvariantMassHist(const std::shared_ptr<GenSimIdentificationModule> genSimModule, const std::shared_ptr<RecoIdentificationModule> recoModule, const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, bool iUsingPhi, bool iMultipleMasses) :
  GenSimRecoPrototype(genSimModule, recoModule, typeSwitch, iname, iNBins, iminimum, imaximum),
  usingPhi(iUsingPhi),
  multipleMasses(iMultipleMasses)
{
}

std::vector<double> SameSignInvariantMassHist::protectedValue(bool typeGenSim) const
{
  if (multipleMasses)
  {
    if (typeGenSim)        // typeGenSim == true, so we want the GenSim values
    {
      auto genParticles = getGenSim()->getGenParticles();
      auto genSimInv = genParticles.calculateSameSignInvariantMasses(usingPhi);
      return {genSimInv};
    }

    else                   // typeGenSim == false, so we want the Reco values
    {
      auto recoParticles = getReco()->getRecoCandidates();
      auto recoInv = recoParticles.calculateSameSignInvariantMasses(usingPhi);
      return {recoInv};
    }
  }

  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getGenSim()->getGenParticles();
    auto genSimInv = genParticles.calculateSameSignInvariantMass(usingPhi);
    return {genSimInv};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getReco()->getRecoCandidates();
    auto recoInv = recoParticles.calculateSameSignInvariantMass(usingPhi);
    return {recoInv};
  }
}
