#ifndef RELISOHIST_HH
#define RELISOHIST_HH

#include "HistogramPrototype1D.hh"

class RelIsoHist : public HistogramPrototype1D
{
  public:
  RelIsoHist(const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;
};


#endif
