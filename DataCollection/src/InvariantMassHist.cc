#include "CMSAnalysis/DataCollection/interface/InvariantMassHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

std::vector<double> InvariantMassHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
    auto genSimInv = genParticles.getInvariantMass();
    return {genSimInv};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getLeptons(InputModule::RecoLevel::Reco);
    auto recoInv = recoParticles.getInvariantMass();
    return {recoInv};
  }
}
