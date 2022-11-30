#ifndef AFBMODULE_HH
#define AFBMODULE_HH

#include "HistogramOutputModule.hh"


// AFBModule fills forward and backward histograms for both gen sim and reco and uses those histograms to calculate the value of Afb at the end
class AFBModule : public HistogramOutputModule
{
public:
  AFBModule(int minMass = 300, int maxMass = 3200, int massInterval = 100);

  virtual void initialize() override;
  virtual bool process() override;
  virtual void finalize() override;

private:
  int minMassCut;
  int maxMassCut;
  int interval;

  // the forward and backward histograms are cut in this # of bins
  const int histBins = 100;

  // fills the respective forward and backward histograms for gen sim over invariant mass
  void fillFB(std::string level, double collinsSoper, double invariantMass); 
  
  // calculates the value of Afb based on the bin contents in the forward and backward histograms
  double getAFB(std::string level, int binNumber); 
};

#endif
