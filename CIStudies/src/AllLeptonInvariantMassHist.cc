#include "CIAnalysis/CIStudies/interface/AllLeptonInvariantMassHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

std::vector<double> AllLeptonInvariantMassHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim);
    auto genSimInv = genParticles.calculateAllLeptonInvariantMass();
    return {genSimInv};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco);
    auto recoInv = recoParticles.calculateAllLeptonInvariantMass();
    return {recoInv};
  }
}
