#ifndef DOUBLEELECTRONTRIGGER_HH
#define DOUBLEELECTRONTRIGGER_HH

#include "RecoTrigger.hh"



class DoubleElectronTrigger : public RecoTrigger
{
  // Trigger Code to analyze HLT Simulations, for double isolated muon triggers
  public:
    DoubleElectronTrigger(const double iPTCutoff = 25, const double iSecondPTCutoff = 25);
  
  protected:
    bool checkTrigger(const InputModule* input) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
};

#endif
