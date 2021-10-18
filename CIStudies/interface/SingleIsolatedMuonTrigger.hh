#ifndef SINGLEISOLATEDMUONTRIGGER_HH
#define SINGLEISOLATEDMUONTRIGGER_HH

#include "Trigger.hh"

class RecoIdentificationModule;

class SingleIsolatedMuonTrigger : public Trigger
{
  public:
    SingleIsolatedMuonTrigger(const double iPTCutoff = 24);
  
  protected:
    bool checkTrigger(std::shared_ptr<InputModule> input) override;

  private:
    const double pTCutoff;
};

#endif
