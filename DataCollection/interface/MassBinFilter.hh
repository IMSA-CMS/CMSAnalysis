#ifndef MASSBINFILTER_HH
#define MASSBINFILTER_HH

#include <memory>
#include "FilterModule.hh"

class MatchingModule;

class MassBinFilter : public FilterModule
{
  public:
    MassBinFilter(const std::shared_ptr<MatchingModule> matchingModule, double iminimum, double imaximum, int iNBins);
  protected:
    virtual std::string makeFilterString() override;
  private:
    const std::shared_ptr<MatchingModule> matching;
    double minimum;
    double maximum;
    int nBins;
};

#endif
