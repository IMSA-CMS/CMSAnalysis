#ifndef DOUBLEMUONTRIGGER_HH
#define DOUBLEMUONTRIGGER_HH

#include "Trigger.hh"
#include "InputModule.hh"

//class RecoIdentificationModule;

class DoubleMuonTrigger : public Trigger
{
  public:
    DoubleMuonTrigger(const double iPTCutoff = 37, const double iSecondPTCutoff = 27);
  
  protected:
    bool checkTrigger(std::shared_ptr<InputModule> input) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
};

#endif
