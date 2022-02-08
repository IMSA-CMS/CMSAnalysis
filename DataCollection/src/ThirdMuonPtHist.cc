#include "CMSAnalysis/DataCollection/interface/ThirdMuonPtHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

std::vector<double> ThirdMuonPtHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
    auto genSimPt = genParticles.getNthHighestPt(3);
    return {genSimPt};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getLeptons(InputModule::RecoLevel::Reco);
    auto recoPt = recoParticles.getNthHighestPt(3);
    return {recoPt};
  }
}
