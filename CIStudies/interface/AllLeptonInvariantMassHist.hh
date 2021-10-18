#ifndef ALLLEPTONINVARIANTMASSHIST_HH
#define ALLLEPTONINVARIANTMASSHIST_HH

#include "CIAnalysis/CIStudies/interface/GenSimRecoPrototype.hh"

class AllLeptonInvariantMassHist : public GenSimRecoPrototype
{
  public:
    using GenSimRecoPrototype::GenSimRecoPrototype;  // Copies the constructor over from GenSimRecoPrototype
  protected:
    std::vector<double> protectedValue(bool typeGenSim) const override;
};

#endif
