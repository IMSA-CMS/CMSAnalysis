#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "FilterModule.hh"

class MatchingModule;

class NLeptonsFilter : public FilterModule
{
  public:
  NLeptonsFilter(const std::shared_ptr<MatchingModule> imatchModule);
  protected:
    virtual std::string makeFilterString(const edm::EventBase& event) override;
  private:
    const std::shared_ptr<MatchingModule> matchModule;
};

#endif
