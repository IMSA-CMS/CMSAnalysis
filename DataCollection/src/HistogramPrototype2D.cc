#include "CMSAnalysis/DataCollection/interface/HistogramPrototype2D.hh"

#include "TH2.h"

HistogramPrototype2D::HistogramPrototype2D(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY) :
  HistogramPrototype(iname),
  nBinsX(iNBinsX),
  nBinsY(iNBinsY),
  minX(iMinX),
  minY(iMinY),
  maxX(iMaxX),
  maxY(iMaxY)
{
}

std::vector<double> HistogramPrototype2D::value() const
{
  auto pairs = value2D();

  std::vector<double> vec;  

  for (auto pair : pairs)
  {
    vec.push_back(pair.first);
    vec.push_back(pair.second);
  }

  return vec;
}

TH2* HistogramPrototype2D::makeHistogram() const
{
  const std::string& name = getFilteredName();
  auto title = getName();
  int nbinsX = getNBinsX();
  int nbinsY = getNBinsY();
  double minX = getMinX();
  double minY = getMinY();
  double maxX = getMaxX();
  double maxY = getMaxY();
  
  return new TH2F(name.c_str(), title.c_str(), nbinsX, minX, maxX, nbinsY, minY, maxY);
}

TH2* HistogramPrototype2D::makeHistogram(std::string name, std::string title) const
{
  int nbinsX = getNBinsX();
  int nbinsY = getNBinsY();
  double minX = getMinX();
  double minY = getMinY();
  double maxX = getMaxX();
  double maxY = getMaxY();
  
  return new TH2F(name.c_str(), title.c_str(), nbinsX, minX, maxX, nbinsY, minY, maxY);
}
