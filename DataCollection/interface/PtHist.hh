#ifndef PTHIST_HH
#define PTHIST_HH

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

class PtHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    std::vector<double> protectedValue(bool typeGenSim) const override;
};

#endif
