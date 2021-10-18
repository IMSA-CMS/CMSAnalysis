#ifndef MVATOTALHIST_HH
#define MVATOTALHIST_HH

#include "HistogramPrototype.hh"

//class GenSimIdentificationModule;

class TotalEventsHist : public HistogramPrototype
{
  public:
  TotalEventsHist(const std::string iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;
};


#endif
