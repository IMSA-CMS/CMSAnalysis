#ifndef MVAFIRSTHIST_HH
#define MVAFIRSTHIST_HH

#include "HistogramPrototype.hh"

class GenSimIdentificationModule;

class MassAcceptanceFirstHist : public HistogramPrototype
{
  public:
  MassAcceptanceFirstHist(const std::shared_ptr<GenSimIdentificationModule> iGenSimIdentificationModule, const std::string iname, int iNBins, double iminimum, double imaximum);
  double value() const override;

  private:
  const std::shared_ptr<GenSimIdentificationModule> genSimIdentification;
};


#endif
