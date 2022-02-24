#include "CMSAnalysis/DataCollection/interface/OppositeSignInvariantMassHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

std::vector<double> OppositeSignInvariantMassHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
    auto genSimInv = genParticles.calculateOppositeSignInvariantMass();
    return {genSimInv};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getLeptons(InputModule::RecoLevel::Reco);
    auto recoInv = recoParticles.calculateOppositeSignInvariantMass();
    return {recoInv};
  }
}
