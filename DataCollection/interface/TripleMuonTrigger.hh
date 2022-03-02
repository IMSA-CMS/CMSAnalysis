#ifndef TRIPLEMUONTRIGGER_HH
#define TRIPLEMUONTRIGGER_HH

#include "RecoTrigger.hh"


class RecoIdentificationModule;

class TripleMuonTrigger : public RecoTrigger
{
  public:
    TripleMuonTrigger(const double iPTCutoff = 10, const double iSecondPTCutoff = 5, const double iThirdPTCutoff = 5);
  
  protected:
    bool checkTrigger(const InputModule* input) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
    const double thirdPTCutoff;
};

#endif
