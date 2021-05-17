#include "CIAnalysis/CIStudies/interface/TriggerModule.hh"

#include "CIAnalysis/CIStudies/interface/Trigger.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

TriggerModule::TriggerModule(std::shared_ptr<RecoIdentificationModule> iRecoMod) :
  recoMod(iRecoMod),
  passAny(0),
  total(0)
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
    if (passCurrentTrigger)  // if (passCurrentTrigger == true)  
    {
      passAnyTrigger = true;
    }
  }

  // Increment the counters (passAny is incremented only if passAnyTrigger is true)
  if (passAnyTrigger)
  {
    ++passAny;
    //std::cout << passAny << '\n';
  }

  ++total;
  //std::cout << total << "\n\n";

  return passAnyTrigger;
}

void TriggerModule::finalize()
{
  for (auto trigger : triggers)
  {
    trigger->printTriggerResult();
    std::cout << '\n';
  }

  printOverallEfficiency();
  std::cout << '\n';
}

void TriggerModule::printOverallEfficiency()
{
  std::cout << "Total event count: " << total << '\n';
  std::cout << "Number of events passing at least one trigger: " << passAny << '\n';
  std::cout << "Overall Efficiency: " << static_cast<double>(passAny) / total << '\n';
}
