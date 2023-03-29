#include "CMSAnalysis/Histograms/interface/SameSignInvariantMassHist.hh"

#include "CMSAnalysis/Filters/interface/Selector.hh"


SameSignInvariantMassHist::SameSignInvariantMassHist(InputModule::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, 
  bool iUsingPhi, bool iMultipleMasses) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  usingPhi(iUsingPhi),
  multipleMasses(iMultipleMasses)
{
}

std::vector<double> SameSignInvariantMassHist::protectedValue(InputModule::RecoLevel typeGenSim) const
{
  if (multipleMasses)
  {
    auto particles = getInput()->getLeptons(typeGenSim);
    // std::cout << "2" << recoParticles.getNumParticles() << std::endl;
    auto inv = particles.calculateSameSignInvariantMasses(usingPhi);
    return {inv};
  } 
  else 
  {
    auto particles = getInput()->getLeptons(typeGenSim);
    // std::cout << "4-" << particles.calculateSameSignInvariantMass() << std::endl;
    auto inv = particles.calculateSameSignInvariantMass();
    return {inv};
  }
}
