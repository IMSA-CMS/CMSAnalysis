#ifndef RECOTRIGGER_HH
#define RECOTRIGGER_HH

#include <memory>
#include "Trigger.hh"

class RecoIdentificationModule;

class RecoTrigger : public Trigger
{
  public:
    RecoTrigger(std::string iName, std::shared_ptr<RecoIdentificationModule> iRecoMod);

  protected:
    virtual bool checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod) = 0;    // Returns true if the trigger is passed
    virtual bool checkTrigger() override {return checkTrigger(recoMod);}

  private:
    std::shared_ptr<RecoIdentificationModule> recoMod;
};

#endif
