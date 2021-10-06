#ifndef DOUBLEISOLATEDMUONTRIGGER_HH
#define DOUBLEISOLATEDMUONTRIGGER_HH

#include "RecoTrigger.hh"

class RecoIdentificationModule;

class DoubleIsolatedMuonTrigger : public RecoTrigger
{
  public:
    DoubleIsolatedMuonTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, const double iPTCutoff = 17, const double iSecondPTCutoff = 8);
  
  protected:
    bool checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
};

#endif
