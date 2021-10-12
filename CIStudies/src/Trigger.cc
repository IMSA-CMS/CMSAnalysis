#include "CIAnalysis/CIStudies/interface/Trigger.hh"

Trigger::Trigger(std::string iName) :
  name(iName)
{
}

bool Trigger::checkEvent()
{
  //std::cerr << "Notice: Trigger::checkEvent is running\n";

  ++total;
  
  if (checkTrigger())
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
