#ifndef HIGGSKANSASSTATENAMEFINDER_HH
#define HIGGSKANSASSTATENAMEFINDER_HH

#include "CMSAnalysis/Analysis/interface/HistNameFinder.hh"

class HiggsKansasStateNameFinder : public HistNameFinder
{
  public:
    HiggsKansasStateNameFinder(std::string channel)
        : channel(channel)
    {}

    std::string getHistName(const HistVariable &histVariable) const override;

  private:
    std::string channel;
};

#endif // HIGGS_HIST_NAME_FINDER_HH