#include "CMSAnalysis/DataCollection/interface/Trigger.hh"

//#include "CMSAnalysis/DataCollection/interface/RecoIdentificationModule.hh"

Trigger::Trigger(std::string iName):
  name(iName)
  // recoMod(iRecoMod)
{
}

bool Trigger::checkEvent(const InputModule* input)
{
  //std::cerr << "Notice: Trigger::checkEvent is running\n";

  ++total;
  
  if (checkTrigger(input))
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
