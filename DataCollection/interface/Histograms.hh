#ifndef HISTOGRAMS_HH
#define HISTOGRAMS_HH

#include "GenSimRecoPrototype.hh"

class AllLeptonInvariantMassHist : GenSimRecoPrototype {
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level)
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = genParticles.calculateAllLeptonInvariantMass();
    return {inv};
  }
};

class InvariantMassHist : GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level)
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = genParticles.getInvariantMass();
    return {inv};
  }
};

class MassResolutionHist : ResolutionPrototype
{
  using ResolutionPrototype::ResolutionPrototype;

  double MassResolutionHist::getValue(const ParticleCollection& partColl) const
  {
    return partColl.getInvariantMass();
  }
};

class OppositeSignInvariantMass : GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> protectedValue(InputModule::RecoLevel level)
  {
    auto particles = getInput()->getLeptons(level);
    auto inv = genParticles.calculateOppositeSignInvariantMass();
    return {inv};
  }
};

class PtHist : GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  std::vector<double> PtHist::protectedValue(InputModule::RecoLevel level) const
  {
    auto genParticles = getInput()->getLeptons(level);
    auto genSimPt = genParticles.getLeadingTransverseMomentum();
    return {genSimPt};
  }
};

class PtResolutionHist : ResolutionPrototype
{
  using ResolutionPrototype::ResolutionPrototype;

  double PtResolutionHist::getValue(const ParticleCollection& partColl) const
  {
    //std::cerr << "congrats you made it" << std::endl;
    return partColl.getLeadingTransverseMomentum();
  }
};

class ThirdMuonPtHist : GenSimRecoPrototype
{
  using GenSimRecoPrototype::GenSimRecoPrototype;

  double ThirdMuonPtHist::protectedValue(InputModule::RecoLevel level)
  {
    auto genParticles = getInput()->getLeptons(level);
    auto Pt = genParticles.getNthHighestPt(3);
    return {Pt};
  }
};

#endif
