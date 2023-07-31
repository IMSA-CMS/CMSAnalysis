#ifndef GENSIMGAMMAHIST_HH
#define GENSIMGAMMAHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"

class GenSimGammaHist : public HistogramPrototype1D
{
  public:
  GenSimGammaHist(const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

};
 
 
#endif
