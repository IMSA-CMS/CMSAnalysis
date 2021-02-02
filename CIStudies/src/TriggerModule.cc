#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"

#include "CIAnalysis/CIStudies/interface/Trigger.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

TriggerModule::TriggerModule(std::shared_ptr<RecoIdentificationModule> iRecoMod) :
  recoMod(iRecoMod)
{
}

bool TriggerModule::process(const edm::EventBase& event)
{
  bool passAnyTrigger = false;       // True if the event passes any of the triggers
  bool passCurrentTrigger;           // True if the event passes the current trigger

  for (auto trigger : triggers)
  {
    passCurrentTrigger = trigger->checkEvent(recoMod);

    // Set passAnyTrigger to true once the event passes one trigger
    // Since passAnyTrigger is automatically false, if the event doesn't pass any triggers, passAnyTrigger remains false
    if (passCurrentTrigger == true)  
    {
      passAnyTrigger = true;
    }
  }

  return passAnyTrigger;
}

void TriggerModule::finalize()
{
  for (auto trigger : triggers)
  {
    trigger->printTriggerResult();
    std::cout << '\n';
  }
}
