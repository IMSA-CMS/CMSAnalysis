#ifndef SCALEFACTORANALYSIS_HH
#define SCALEFACTORANALYSIS_HH

#include "HistogramOutputModule.hh"
#include <vector>

// ScaleFactorAnalysis processes histograms with and without scale factors applied, 
// computes bin-by-bin residuals, and outputs relevant histograms.
class ScaleFactorAnalysis : public HistogramOutputModule 
{
public:
    // Constructor with default parameters for histogram range and binning
    ScaleFactorAnalysis(int low = 0, int high = 1000, int histBins = 1000);

    // Overrides from HistogramOutputModule
    virtual void initialize() override;
    virtual bool process() override;
    virtual void finalize() override;

private:
    // Histogram parameters
    Double_t low = 0;       // Lower range of histogram
    Double_t high = 1000;   // Upper range of histogram
    Int_t histBins = 1000;  // Number of bins in the histograms

    // Default histogram (unweighted)
    TH1* defHist;

    // Histograms for analysis
    TH1F* withScaleHist;        // Histogram with scale factors applied
    TH1F* withoutScaleHist;     // Histogram without scale factors applied
    TH1F* withScaleResHist;     // Residuals with scale factors
    TH1F* withoutScaleResHist;  // Residuals without scale factors

    // Vector of histograms for PDF weights
    std::vector<TH1*> histVec;

    // Stores bin ratios for all histograms
    std::vector<std::vector<double>> histogramBinRatios;
};

#endif
