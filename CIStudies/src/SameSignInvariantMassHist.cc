#include "CIAnalysis/CIStudies/interface/SameSignInvariantMassHist.hh"

#include "CIAnalysis/CIStudies/interface/Selector.hh"


SameSignInvariantMassHist::SameSignInvariantMassHist(const bool typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, 
  bool iUsingPhi, bool iMultipleMasses) :
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
      auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
      // std::cout << genParticles.getNumParticles() << std::endl;
      auto genSimInv = genParticles.calculateSameSignInvariantMasses(usingPhi);
      return {genSimInv};
    }

    else                   // typeGenSim == false, so we want the Reco values
    {
      auto recoParticles = getInput()->getLeptons(InputModule::RecoLevel::Reco);
      // std::cout << "2" << recoParticles.getNumParticles() << std::endl;
      auto recoInv = recoParticles.calculateSameSignInvariantMasses(usingPhi);
      return {recoInv};
    }
  }

  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
    // std::cout << "3" << genParticles.getNumParticles() << std::endl;
    auto genSimInv = genParticles.calculateSameSignInvariantMass();
    return {genSimInv};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getLeptons(InputModule::RecoLevel::Reco);
    // std::cout << "4-" << recoParticles.calculateSameSignInvariantMass() << std::endl;
    auto recoInv = recoParticles.calculateSameSignInvariantMass();
    return {recoInv};
  }
}
