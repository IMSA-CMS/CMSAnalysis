#ifndef PDFANALYSISMODULE_HH
#define PDFANALYSISMODULE_HH

#include "HistogramOutputModule.hh"


// AFBModule fills forward and backward histograms for both gen sim and reco and uses those histograms to calculate the value of Afb at the end
class PdfAnalysisModule : public HistogramOutputModule 
{
public:
  PdfAnalysisModule(int low = 0, int high= 1000, int histBins = 1000);

  virtual void initialize() override;
  virtual bool process() override;
  virtual void finalize() override;

private:
  Double_t low=0;
  Double_t high=1000;
  //# of bins
  Int_t histBins = 1000;
  std::vector<TH1*> histVec;
  std::vector<std::vector<double>> histogramBinRatios;
  TH1* defHist;

};

#endif