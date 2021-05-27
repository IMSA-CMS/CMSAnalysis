#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "FilterModule.hh"

class RecoIdentificationModule;

class NLeptonsFilter : public FilterModule
{
  public:
  NLeptonsFilter(const std::shared_ptr<RecoIdentificationModule> iRecoModule);
  protected:
    virtual std::string makeFilterString(const edm::EventBase& event) override;
  private:
    const std::shared_ptr<RecoIdentificationModule> recoModule;
};

#endif
