#include "CMSAnalysis/DataCollection/interface/PtHist.hh"

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

std::vector<double> PtHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    auto genParticles = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
    auto genSimPt = genParticles.getLeadingTransverseMomentum();
    return {genSimPt};
  }

  else                     // typeGenSim == false, so we want the Reco values
  {
    auto recoParticles = getInput()->getLeptons(InputModule::RecoLevel::Reco);
    auto recoPt = recoParticles.getLeadingTransverseMomentum();
    return {recoPt};
  }
}