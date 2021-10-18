#ifndef SINGLEELECTRONTRIGGER_HH
#define SINGLEELECTRONTRIGGER_HH

#include "Trigger.hh"

//class RecoIdentificationModule;

class SingleElectronTrigger : public Trigger
{
  public:
    SingleElectronTrigger(const double iPTCutoff = 28);
  
  protected:
    bool checkTrigger(std::shared_ptr<InputModule> input) override;

  private:
    const double pTCutoff;
};

#endif
