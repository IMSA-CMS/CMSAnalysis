#ifndef DOUBLEISOLATEDMUONTRIGGER_HH
#define DOUBLEISOLATEDMUONTRIGGER_HH

#include "RecoTrigger.hh"

class DoubleIsolatedMuonTrigger : public RecoTrigger
{
  // Trigger Code to analyze HLT Simulations, for double isolated muons  
  public:
    DoubleIsolatedMuonTrigger(const double iPTCutoff = 17, const double iSecondPTCutoff = 8);
  
  protected:
    bool checkTrigger(const InputModule* input) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
};

#endif
