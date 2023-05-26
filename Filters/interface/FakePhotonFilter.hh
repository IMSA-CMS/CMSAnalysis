#ifndef FAKEPHOTONFILTER_HH
#define FAKEPHOTONFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"

class MatchingModule;

class FakePhotonFilter : public Filter
{
  public:
    FakePhotonFilter();
  protected:
        virtual std::string getFilterString(const InputModule* inputMod) const override;
  private:
};

#endif
