#ifndef SINGLEISOLATEDMUONTRIGGER_HH
#define SINGLEISOLATEDMUONTRIGGER_HH

#include "RecoTrigger.hh"


class SingleIsolatedMuonTrigger : public RecoTrigger
{
  public:
    SingleIsolatedMuonTrigger(const double iPTCutoff = 24);
  
  protected:
    bool checkTrigger(const InputModule* input) override;

  private:
    const double pTCutoff;
};

#endif