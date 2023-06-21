#ifndef FAKEPHOTONFILTER_HH
#define FAKEPHOTONFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

class MatchingModule;

class FakePhotonFilter : public Filter
{
  public:
    FakePhotonFilter();
  protected:
        virtual std::string getFilterString(const EventInput* inputMod) const override;
  private:
};

#endif
