#include "CIAnalysis/CIStudies/interface/SameSignInvariantMassHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

SameSignInvariantMassHist::SameSignInvariantMassHist(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, bool iUsingPhi, bool iMultipleMasses) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
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
      auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim);
      auto genSimInv = genParticles.calculateSameSignInvariantMasses(usingPhi);
      return {genSimInv};
    }

    else                   // typeGenSim == false, so we want the Reco values
    {
      auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco);
      auto recoInv = recoParticles.calculateSameSignInvariantMasses(usingPhi);
      return {recoInv};
    }
  }

  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim);
    auto genSimInv = genParticles.calculateSameSignInvariantMass();
    return {genSimInv};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco);
    auto recoInv = recoParticles.calculateSameSignInvariantMass();
    return {recoInv};
  }
}
