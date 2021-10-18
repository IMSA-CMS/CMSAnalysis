#ifndef DOUBLEISOLATEDMUONTRIGGER_HH
#define DOUBLEISOLATEDMUONTRIGGER_HH

#include "Trigger.hh"

//class RecoIdentificationModule;

class DoubleIsolatedMuonTrigger : public Trigger
{
  public:
    DoubleIsolatedMuonTrigger(const double iPTCutoff = 17, const double iSecondPTCutoff = 8);
  
  protected:
    bool checkTrigger(std::shared_ptr<InputModule> input) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
};

#endif
