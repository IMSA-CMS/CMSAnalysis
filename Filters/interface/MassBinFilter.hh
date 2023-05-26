#ifndef MASSBINFILTER_HH
#define MASSBINFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"

class MatchingModule;

class MassBinFilter : public Filter
{
  public:
    MassBinFilter(const std::shared_ptr<MatchingModule> matchingModule, double iminimum, double imaximum, int iNBins);
  protected:
    virtual std::string getFilterString(const InputModule* inputMod) const override;
  private:
    const std::shared_ptr<MatchingModule> matching;
    double minimum;
    double maximum;
    int nBins;
};

#endif
