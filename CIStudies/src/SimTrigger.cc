#include "CIAnalysis/CIStudies/interface/SimTrigger.hh"

SimTrigger::SimTrigger(std::string iName, TriggerSimModule::EnumTriggers iTriggerEnum, std::shared_ptr<TriggerSimModule> iSimMod) :
  Trigger(iName),
  triggerEnum(iTriggerEnum),
  simMod(iSimMod)
{
  simMod->addTrigger(triggerEnum);
}