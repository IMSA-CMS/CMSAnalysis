#ifndef SINGLEMUONTRIGGER_HH
#define SINGLEMUONTRIGGER_HH

#include "RecoTrigger.hh"


class SingleMuonTrigger : public RecoTrigger
{
  public:
    SingleMuonTrigger(const double iPTCutoff = 50);
  
  protected:
    bool checkTrigger(std::shared_ptr<InputModule> input) override;

  private:
    const double pTCutoff;
};

#endif
