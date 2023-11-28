#ifndef PDFHISTOGRAMSET_HH
#define PDFHISTOGRAMSET_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype.hh"

class PDFHistogramSet
{
public:
    PDFHistogramSet(std::shared_ptr<HistogramPrototype> histogram);
    std::vector<double> getTotalBinUncertainties();
    double getBinUncertainty(int bin);

private:
    std::vector<double> getSigmas(int bin);
    std::vector<std::shared_ptr<HistogramPrototype>> pdfHistograms;
};

#endif