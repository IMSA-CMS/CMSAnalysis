#ifndef DOUBLEMUONTRIGGER_HH
#define DOUBLEMUONTRIGGER_HH

#include "RecoTrigger.hh"

class RecoIdentificationModule;

class DoubleMuonTrigger : public RecoTrigger
{
  public:
    DoubleMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, const double iPTCutoff = 37, const double iSecondPTCutoff = 27);
  
  protected:
    bool checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
};

#endif
