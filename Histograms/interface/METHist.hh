#ifndef METHIST_HH
#define METHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"


class METHist : public HistogramPrototype1D
{
  public:
  METHist( const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:


};

#endif