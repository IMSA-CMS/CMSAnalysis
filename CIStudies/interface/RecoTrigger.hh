#ifndef RECOTRIGGER_HH
#define RECOTRIGGER_HH

#include <memory>
#include "Trigger.hh"

class RecoTrigger : public Trigger
{
  public:
    RecoTrigger(std::string iName);

  protected:
    virtual bool checkTrigger(std::shared_ptr<InputModule> input) = 0;    // Returns true if the trigger is passed
};

#endif
