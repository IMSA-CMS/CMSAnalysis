#include "CMSAnalysis/Histograms/interface/SameSignInvariantMassHist.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"


SameSignInvariantMassHist::SameSignInvariantMassHist(EventInput::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, 
  bool useCorrectedLeptons) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  corrected(useCorrectedLeptons)
{
}

ParticleCollection<Lepton> SameSignInvariantMassHist::getCorrectedLeptons() const
{
  ParticleCollection<Lepton> response;
  // std::cout << "Getting corrected leptons for TwoInvariantMassesHist\n";
  auto electrons = getInput()->getSpecial("correctedElectron");
  for (auto electron : electrons)
  {
    response.addParticle(Lepton(electron));
  }
  auto muons = getInput()->getSpecial("correctedMuon");
  for (auto muon : muons)
  {
    response.addParticle(Lepton(muon));
  }
  auto taus = getInput()->getSpecial("correctedTau");
  for (auto tau : taus)
  {
    response.addParticle(Lepton(tau));
  }
  // std::cout << "Got " << response.size() << " corrected leptons for TwoInvariantMassesHist\n";
  return response;
}

std::vector<double> SameSignInvariantMassHist::protectedValue(EventInput::RecoLevel typeGenSim) const
{
  auto particles = corrected ? getCorrectedLeptons() : getInput()->getLeptons(typeGenSim);
  // std::cout << particles.getNumParticles() << std::endl;
  // std::cout << "4-" << particles.calculateSameSignInvariantMass() << std::endl;
  auto inv = particles.calculateSameSignInvariantMass();
  // std::cout << inv << std::endl;
  return {inv};
}
