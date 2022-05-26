#ifndef THIRDMUONPTHIST_HH
#define THIRDMUONPTHIST_HH

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

class ThirdMuonPtHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;
  protected:
    std::vector<double> protectedValue(bool typeGenSim) const override;
};

#endif
