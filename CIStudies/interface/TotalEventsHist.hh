#ifndef MVATOTALHIST_HH
#define MVATOTALHIST_HH

#include "HistogramPrototype.hh"

class GenSimIdentificationModule;

class TotalEventsHist : public HistogramPrototype
{
  public:
  TotalEventsHist(const std::shared_ptr<GenSimIdentificationModule> iGenSimIdentificationModule, const std::string iname, int iNBins, double iminimum, double imaximum);
  double value() const override;

  private:
  const std::shared_ptr<GenSimIdentificationModule> genSimIdentification;
};


#endif
