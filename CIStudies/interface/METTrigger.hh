#ifndef METTRIGGER_HH
#define METTRIGGER_HH

#include "RecoTrigger.hh"

class METModule;

class METTrigger : public RecoTrigger
{
  public:
    METTrigger(std::shared_ptr<METModule> iMETMod, const double iMETCutoff = 120);

  protected:
    bool checkTrigger(std::shared_ptr<RecoIdentificationModule>) override;

  private:
    const std::shared_ptr<METModule> mETMod;
    const double mETCutoff;
};

#endif
