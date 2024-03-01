#ifndef AFBMODULE_HH
#define AFBMODULE_HH

#include "HistogramOutputModule.hh"


// AFBModule fills forward and backward histograms for both gen sim and reco and uses those histograms to calculate the value of Afb at the end
class AFBModule : public HistogramOutputModule
{
public:
  AFBModule(int low = 300, int high = 3200, int histBins = 1000);

  virtual void initialize() override;
  virtual bool process() override;
  virtual void finalize() override;

private:
  int low;
  int high;
  // the forward and backward histograms are cut in this # of bins
  const int histBins = 1000;

};

#endif
