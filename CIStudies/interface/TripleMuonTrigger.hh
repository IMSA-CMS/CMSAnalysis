#ifndef TRIPLEMUONTRIGGER_HH
#define TRIPLEMUONTRIGGER_HH

#include "Trigger.hh"

class RecoIdentificationModule;

class TripleMuonTrigger : public Trigger
{
  public:
    TripleMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, const double iPTCutoff = 10, const double iSecondPTCutoff = 5, const double iThirdPTCutoff = 5);
  
  protected:
    bool checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
    const double thirdPTCutoff;
};

#endif
