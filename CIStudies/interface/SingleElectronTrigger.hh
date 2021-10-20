#ifndef SINGLEELECTRONTRIGGER_HH
#define SINGLEELECTRONTRIGGER_HH

#include "RecoTrigger.hh"

class RecoIdentificationModule;

class SingleElectronTrigger : public RecoTrigger
{
  public:
    SingleElectronTrigger(std::shared_ptr<RecoIdentificationModule> iRecoMod, const double iPTCutoff = 28);
  
  protected:
    bool checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod) override;

  private:
    const double pTCutoff;
};

#endif
