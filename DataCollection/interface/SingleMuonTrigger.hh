#ifndef SINGLEMUONTRIGGER_HH
#define SINGLEMUONTRIGGER_HH

#include "RecoTrigger.hh"


class SingleMuonTrigger : public RecoTrigger
{
  public:
    SingleMuonTrigger(const double iPTCutoff = 50);
  
  protected:
    bool checkTrigger(const InputModule* input) override; //checks if event passes trigger criteria

  private:
    const double pTCutoff;
};

#endif
