#ifndef SINGLEELECTRONTRIGGER_HH
#define SINGLEELECTRONTRIGGER_HH

#include "RecoTrigger.hh"

//class RecoIdentificationModule;

class SingleElectronTrigger : public RecoTrigger
{
  public:
    SingleElectronTrigger(const double iPTCutoff = 28);
  
  protected:
    bool checkTrigger(const InputModule* input) override;

  private:
    const double pTCutoff;
};

#endif
