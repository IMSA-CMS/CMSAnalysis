#ifndef MASSBINFILTER_HH
#define MASSBINFILTER_HH

#include "FilterModule.hh"

class MatchingModule;

class MassBinFilter : public FilterModule
{
  public:
    MassBinFilter(const MatchingModule& matchingModule, double iminimum, double imaximum, int iNBins);
  protected:
    virtual std::string makeFilterString(const edm::EventBase& event) override;
  private:
    const MatchingModule& matching;
    double minimum;
    double maximum;
    int nBins;
};

#endif
