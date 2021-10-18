#ifndef MVAFIRSTHIST_HH
#define MVAFIRSTHIST_HH

#include "HistogramPrototype.hh"

class GenSimIdentificationModule;

class MassAcceptanceFirstHist : public HistogramPrototype
{
  public:
  MassAcceptanceFirstHist(const std::string iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;
};


#endif
