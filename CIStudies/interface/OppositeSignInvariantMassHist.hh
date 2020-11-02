#ifndef OPPOSITESIGNINVARIANTMASSHIST_HH
#define OPPOSITESIGNINVARIANTMASSHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class OppositeSignInvariantMassHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    double protectedValue(bool typeGenSim) const override;
};

#endif
