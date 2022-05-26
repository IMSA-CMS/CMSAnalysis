#ifndef INVARIANTMASSHIST_HH
#define INVARIANTMASSHIST_HH

#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"

class InvariantMassHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    std::vector<double> protectedValue(bool typeGenSim) const override;
};

#endif
