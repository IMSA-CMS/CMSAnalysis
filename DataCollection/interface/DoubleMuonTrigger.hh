#ifndef DOUBLEMUONTRIGGER_HH
#define DOUBLEMUONTRIGGER_HH

#include "Trigger.hh"
#include "InputModule.hh"
#include "RecoTrigger.hh"

  // Trigger Code to analyze HLT Simulations, for double muons
class DoubleMuonTrigger : public RecoTrigger
{
  public:
    DoubleMuonTrigger(const double iPTCutoff = 37, const double iSecondPTCutoff = 27);
  
  protected:
    bool checkTrigger(const InputModule* input) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
};

#endif
