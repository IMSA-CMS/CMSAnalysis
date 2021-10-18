#include "CIAnalysis/CIStudies/interface/InvariantMassHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

std::vector<double> InvariantMassHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim);
    auto genSimInv = genParticles.getInvariantMass();
    return {genSimInv};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco);
    auto recoInv = recoParticles.getInvariantMass();
    return {recoInv};
  }
}
