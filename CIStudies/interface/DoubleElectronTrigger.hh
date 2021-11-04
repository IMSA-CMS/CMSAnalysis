#ifndef DOUBLEELECTRONTRIGGER_HH
#define DOUBLEELECTRONTRIGGER_HH

#include "RecoTrigger.hh"

//class RecoIdentificationModule;

class DoubleElectronTrigger : public RecoTrigger
{
  public:
    DoubleElectronTrigger(const double iPTCutoff = 25, const double iSecondPTCutoff = 25);
  
  protected:
    bool checkTrigger(std::shared_ptr<InputModule> input) override;

  private:
    const double pTCutoff;
    const double secondPTCutoff;
};

#endif
