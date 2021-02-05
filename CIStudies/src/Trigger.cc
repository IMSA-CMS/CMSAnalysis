#include "CIAnalysis/CIStudies/interface/Trigger.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

Trigger::Trigger(std::string iName, std::shared_ptr<RecoIdentificationModule> iRecoMod) :
  name(iName),
  recoMod(iRecoMod)
{
}

bool Trigger::checkEvent(std::shared_ptr<RecoIdentificationModule> recoMod)
{
  ++total;
  
  if (checkTrigger(recoMod))
  {
    ++pass;
    return true;
  }

  else
  {
    return false;
  }
}

void Trigger::printTriggerResult()
{
  std::cout << name << '\n';
  std::cout << "Total number: " << total << '\n';
  std::cout << "Total passed: " << pass << '\n';
  std::cout << "Pass ratio: " << passRatio() << '\n';
}
