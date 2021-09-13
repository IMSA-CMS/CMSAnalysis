#ifndef SINGLEMUONTRIGGER_HH
#define SINGLEMUONTRIGGER_HH

#include "RecoTrigger.hh"

class RecoIdentificationModule;

class SingleMuonTrigger : public RecoTrigger
{
  public:
    SingleMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, const double iPTCutoff = 50);
  
  protected:
    bool checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod) override;

  private:
    const double pTCutoff;
};

#endif
