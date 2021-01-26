#ifndef TRIGGER_HH
#define TRIGGER_HH

#include <memory>

class RecoIdentificationModule;

class Trigger
{
  public:
    Trigger(std::shared_ptr<RecoIdentificationModule> iRecoMod);
    bool checkEvent(std::shared_ptr<RecoIdentificationModule> recoMod);                  // Calls checkTrigger() and increments pass and total
    virtual ~Trigger() {};

  protected:
    virtual bool checkTrigger(std::shared_ptr<RecoIdentificationModule> recoMod) = 0;    // Returns true if the trigger is passed

  private:
    int pass = 0;
    int total = 0;
    std::shared_ptr<RecoIdentificationModule> recoMod;
};

#endif
