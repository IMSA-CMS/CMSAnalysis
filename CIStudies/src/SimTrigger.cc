#include "CIAnalysis/CIStudies/interface/SimTrigger.hh"

SimTrigger::SimTrigger(std::string iName, TriggerSimModule::EnumTriggers iTriggerEnum, std::shared_ptr<TriggerSimModule> iTrigSimMod) :
  Trigger(iName),
  triggerEnum(iTriggerEnum),
  trigSimMod(iTrigSimMod)
{
  trigSimMod->addTrigger(triggerEnum);
}