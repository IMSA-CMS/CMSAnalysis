#ifndef HISTOGRAMPROTOTYPE1D_HH
#define HISTOGRAMPROTOTYPE1D_HH

#include "HistogramPrototype.hh"

#include "TH1.h"

#include <string>

class HistogramPrototype1D : public HistogramPrototype
{
  public:
    HistogramPrototype1D(const std::string& iname, int inBins, double iminimum, double imaximum);
    int getNBins() const {return nBins;}
    double getMinimum() const {return minimum;}
    double getMaximum() const {return maximum;}
    TH1* makeHistogram() const override;
    TH1* makeHistogram(std::string name, std::string title) const override;
  private:
    int nBins;
    double minimum;
    double maximum;
};

#endif
