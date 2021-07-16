#ifndef UNUSUALFINALSTATEFILTER_HH
#define UNUSUALFINALSTATEFILTER_HH

#include <memory>
#include "FilterModule.hh"

class RecoIdentificationModule;

class UnusualFinalStateFilter : public FilterModule
{
  public:
    UnusualFinalStateFilter(const std::shared_ptr<RecoIdentificationModule> iReco);
  protected:
    virtual std::string makeFilterString(const edm::EventBase& event) override;
  private:
    const std::shared_ptr<RecoIdentificationModule> reco;
};

#endif
