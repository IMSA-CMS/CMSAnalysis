#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "FilterModule.hh"

//class RecoIdentificationModule;

class NLeptonsFilter : public FilterModule
{
  public:
  NLeptonsFilter();
  protected:
    virtual std::string makeFilterString() override;
  private:
    // const std::shared_ptr<RecoIdentificationModule> recoModule;
};

#endif
