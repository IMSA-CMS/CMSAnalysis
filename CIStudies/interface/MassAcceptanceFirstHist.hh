#ifndef MVAFIRSTHIST_HH
#define MVAFIRSTHIST_HH

#include "HistogramPrototype1D.hh"

class GenSimIdentificationModule;

class MassAcceptanceFirstHist : public HistogramPrototype1D
{
  public:
  MassAcceptanceFirstHist(const std::shared_ptr<GenSimIdentificationModule> iGenSimIdentificationModule, const std::string iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  const std::shared_ptr<GenSimIdentificationModule> genSimIdentification;
};


#endif
