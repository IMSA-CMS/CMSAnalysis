#ifndef SIMTRIGGER_HH
#define SIMTRIGGER_HH

#include <memory>
#include "Trigger.hh"
#include "TriggerSimModule.hh"

class SimTrigger : public Trigger
{
  public:
    SimTrigger(std::string iName, TriggerSimModule::EnumTriggers iTriggerEnum, std::shared_ptr<TriggerSimModule> iSimMod);
    virtual std::vector<std::string> getNames() const {return simMod->getNames();}

  protected:
    virtual bool checkTrigger() override {return simMod->checkTrigger(triggerEnum);} // Returns true if the trigger is passed

  private:
    TriggerSimModule::EnumTriggers triggerEnum;
    std::shared_ptr<TriggerSimModule> simMod;
};

#endif
