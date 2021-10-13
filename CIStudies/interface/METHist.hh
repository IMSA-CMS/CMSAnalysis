#ifndef METHIST_HH
#define METHIST_HH

#include "HistogramPrototype1D.hh"

class METModule;

class METHist : public HistogramPrototype1D
{
  public:
  METHist(std::shared_ptr<METModule> iMETModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  const std::shared_ptr<METModule> mETModule;

};

#endif