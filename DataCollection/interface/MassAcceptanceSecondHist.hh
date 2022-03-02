#ifndef MVASECONDHIST_HH
#define MVASECONDHIST_HH

#include "HistogramPrototype1D.hh"

class MatchingModule;

class MassAcceptanceSecondHist : public HistogramPrototype1D
{
  public:
  MassAcceptanceSecondHist(const std::shared_ptr<MatchingModule> iMatchingModule, const std::string iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  const std::shared_ptr<MatchingModule> matchMod;
};


#endif
