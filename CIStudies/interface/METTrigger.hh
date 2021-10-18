#ifndef METTRIGGER_HH
#define METTRIGGER_HH

#include "Trigger.hh"

class METModule;

class METTrigger : public Trigger
{
  public:
    METTrigger(std::shared_ptr<METModule> iMETMod, const double iMETCutoff = 120);

  protected:
    bool checkTrigger(std::shared_ptr<InputModule> input) override;

  private:
    const std::shared_ptr<METModule> mETMod;
    const double mETCutoff;
};

#endif
