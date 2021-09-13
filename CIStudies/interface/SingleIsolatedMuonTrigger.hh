#ifndef SINGLEISOLATEDMUONTRIGGER_HH
#define SINGLEISOLATEDMUONTRIGGER_HH

#include "RecoTrigger.hh"

class RecoIdentificationModule;

class SingleIsolatedMuonTrigger : public RecoTrigger
{
  public:
    SingleIsolatedMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, const double iPTCutoff = 24);
  
  protected:
    bool checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod) override;

  private:
    const double pTCutoff;
};

#endif
