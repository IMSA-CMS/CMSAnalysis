#ifndef MASSETA2DHIST_HH
#define MASSETA2DHIST_HH

#include "CMSAnalysis/Histograms/interface/HistogramPrototype2DProjection.hh"
#include <string>
#include <vector>

class MassEta2Dhist : public HistogramPrototype2D
{
public:
  MassEta2Dhist(const std::string &iname,
                int iNBinsX, int iNBinsY,
                double iMinX, double iMinY,
                double iMaxX, double iMaxY);

  virtual std::vector<std::pair<double, double>> value2D() const override;
};

#endif
