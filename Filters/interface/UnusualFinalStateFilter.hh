#ifndef UNUSUALFINALSTATEFILTER_HH
#define UNUSUALFINALSTATEFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"

class RecoIdentificationModule;

class UnusualFinalStateFilter : public Filter
{
  public:
    UnusualFinalStateFilter();
  protected:
    virtual std::string getFilterString(const EventInput* inputMod) const override;
  private:
};

#endif
