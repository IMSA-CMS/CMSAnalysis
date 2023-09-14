#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "TH1.h"

HistogramPrototype1D::HistogramPrototype1D(const std::string& iname, int inBins, double iminimum, double imaximum) :
  HistogramPrototype(iname),
  nBins(inBins),
  minimum(iminimum),
  maximum(imaximum)
{
}

TH1* HistogramPrototype1D::makeHistogram() const
{
  const std::string& name = getFilteredName();
  auto title = getName();
  int nbins = getNBins();
  double min = getMinimum();
  double max = getMaximum();
  
  return new TH1F(name.c_str(), title.c_str(), nbins, min, max);
}

TH1* HistogramPrototype1D::makeHistogram(std::string name, std::string title) const
{
  int nbins = getNBins();
  double min = getMinimum();
  double max = getMaximum();
  
  return new TH1F(name.c_str(), title.c_str(), nbins, min, max);
}
