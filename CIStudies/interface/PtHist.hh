#ifndef PTHIST_HH
#define PTHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class PtHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    double protectedValue(bool typeGenSim) const override;
};

#endif
