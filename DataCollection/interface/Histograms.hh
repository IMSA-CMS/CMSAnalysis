#ifndef HISTOGRAMS_HH
#define HISTOGRAMS_HH

#include "GenSimRecoPrototype.hh"

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

#endif
