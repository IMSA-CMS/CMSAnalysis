#ifndef MASSPT2DHIST_HH
#define MASSPT2DHIST_HH

#include "CMSAnalysis/Histograms/interface/HistogramPrototype2DProjection.hh"
#include <string>
#include <vector>

class MassPt2Dhist : public HistogramPrototype2D
{
public:
  MassPt2Dhist(const std::string &iname,
               int iNBinsX, int iNBinsY,
               double iMinX, double iMinY,
               double iMaxX, double iMaxY);

  virtual std::vector<std::pair<double, double>> value2D() const override;
};

#endif
