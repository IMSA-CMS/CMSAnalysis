#ifndef INVARIANTMASSHIST_HH
#define INVARIANTMASSHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class InvariantMassHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    double protectedValue(bool typeGenSim) const override;
};

#endif
