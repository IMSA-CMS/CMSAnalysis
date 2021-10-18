#include "CIAnalysis/CIStudies/interface/PhotonsHist.hh"
#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

#include <vector>

std::vector<double> PhotonsHist::protectedValue(bool typeGenSim) const
{
  if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
  {
    std::vector<double> ptVector;
    auto genPhotons = getInput()->getParticles(InputModule::RecoLevel::GenSim, Particle::Type::Photon).getParticles();
    for(auto currentPhoton:genPhotons)
    {
      auto genPhotonPt = currentPhoton.pt();
      ptVector.push_back(genPhotonPt);
    }
    return ptVector;
  }
  return {};
}