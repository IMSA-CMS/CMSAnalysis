#ifndef METTRIGGER_HH
#define METTRIGGER_HH

#include "RecoTrigger.hh"

class METModule;

class METTrigger : public RecoTrigger
{
  public:
    METTrigger(std::shared_ptr<METModule> iMETMod, const double iMETCutoff = 120);

  protected:
    bool checkTrigger(const EventInput* input) override; //checks if event passes trigger criteria

  private:
    const std::shared_ptr<METModule> mETMod;
    const double mETCutoff;
};

#endif
