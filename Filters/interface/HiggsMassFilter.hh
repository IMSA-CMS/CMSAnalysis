#ifndef HIGGSMASSFILTER_HH
#define HIGGSMASSFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"

class HiggsMassFilter : public Filter
{
  public:
    HiggsMassFilter(double cutoff) : cutoff(cutoff)
    {
    }
    std::string getFilterString(const EventInput *input) const override;

  private:
    double cutoff;
};

#endif