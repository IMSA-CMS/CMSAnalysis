#ifndef SIMTRIGGER_HH
#define SIMTRIGGER_HH

#include <memory>
#include "Trigger.hh"
#include "TriggerSimModule.hh"

class SimTrigger : public Trigger
{
  public:
    SimTrigger(std::string iName, TriggerSimModule::EnumTriggers iTriggerEnum, std::shared_ptr<TriggerSimModule> iTrigSimMod);
    virtual std::vector<std::string> getNames() const {return trigSimMod->getNames();}
    
    // For debugging purposes
    void enableAllTriggers() {trigSimMod->enableAllTriggers();}

  protected:
    virtual bool checkTrigger(std::shared_ptr<InputModule> input) override {return trigSimMod->checkTrigger(triggerEnum);} // Returns true if the trigger is passed

  private:
    TriggerSimModule::EnumTriggers triggerEnum;
    std::shared_ptr<TriggerSimModule> trigSimMod;
};

#endif
