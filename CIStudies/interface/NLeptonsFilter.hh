#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include "FilterModule.hh"

class MatchingModule;

class NLeptonsFilter : public FilterModule
{
  public:
    NLeptonsFilter(const MatchingModule& imatchModule);
  protected:
    virtual std::string makeFilterString(const edm::EventBase& event) override;
  private:
    const MatchingModule& matchModule;
};

#endif
