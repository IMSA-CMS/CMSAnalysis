#ifndef GETNTHHIGHESTPTHIST_HH
#define GETNTHHIGHESTPTHIST_HH

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

class GetNthHighestPtHist : public GenSimRecoPrototype
{
  public:
    GetNthHighestPtHist(InputModule::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon = 1, Particle::Type parType = Particle::Type::None);
    using GenSimRecoPrototype::GenSimRecoPrototype;
    

  protected:
    std::vector<double> protectedValue(InputModule::RecoLevel typeGenSim) const override;

  private:
    int nthMuon;
    Particle::Type particleType;
};

#endif
