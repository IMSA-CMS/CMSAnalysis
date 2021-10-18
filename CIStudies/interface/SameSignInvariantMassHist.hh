#ifndef SAMESIGNINVARIANTMASSHIST_HH
#define SAMESIGNINVARIANTMASSHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class SameSignInvariantMassHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    virtual std::vector<double> protectedValue(bool typeGenSim) const override;
};

#endif
