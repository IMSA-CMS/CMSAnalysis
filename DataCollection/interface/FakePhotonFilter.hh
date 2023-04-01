#ifndef FAKEPHOTONFILTER_HH
#define FAKEPHOTONFILTER_HH

#include <memory>
#include "FilterModule.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"

class MatchingModule;

class FakePhotonFilter : public FilterModule
{
  public:
    FakePhotonFilter();
  protected:
    virtual std::string makeFilterString() override;
  private:
};

#endif
