#include "CIAnalysis/CIStudies/interface/ThirdMuonPtHist.hh"

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

std::vector<double> ThirdMuonPtHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getParticles(InputModule::RecoLevel::GenSim);
    auto genSimPt = genParticles.getNthHighestPt(3);
    return {genSimPt};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getParticles(InputModule::RecoLevel::Reco);
    auto recoPt = recoParticles.getNthHighestPt(3);
    return {recoPt};
  }
}
