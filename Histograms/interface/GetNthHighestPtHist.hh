#ifndef GETNTHHIGHESTPTHIST_HH
#define GETNTHHIGHESTPTHIST_HH

#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

class GetNthHighestPtHist : public GenSimRecoPrototype
{
  public:
    GetNthHighestPtHist(EventInput::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon = 1, const ParticleType& parType = ParticleType::none());
    using GenSimRecoPrototype::GenSimRecoPrototype;
    

  protected:
    std::vector<double> protectedValue(EventInput::RecoLevel typeGenSim) const override;

  private:
    int nthMuon;
    const ParticleType& particleType;
};

#endif
