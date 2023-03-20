#ifndef DARKPHOTONMASSHIST_HH
#define DARKPHOTONMASSHIST_HH

#include "HistogramPrototype1D.hh"

class DarkPhotonMassHist : public HistogramPrototype1D
{
  public:
  DarkPhotonMassHist(const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

};
  
 
#endif
