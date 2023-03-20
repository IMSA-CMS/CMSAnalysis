T#ifndef GENSIMDELTARHIST_HH
#define GENSIMDELTARHIST_HH

#include "HistogramPrototype1D.hh"

class GenSimDeltaRHist : public HistogramPrototype1D
{
  public:
  GenSimDeltaRHist(const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

};


#endif
