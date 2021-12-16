#ifndef TWOINVARIANTMASSESHIST_HH
#define TWOINVARIANTMASSESHIST_HH

#include "CIAnalysis/CIStudies/interface/HistogramPrototype2D.hh"

class Selector;

class TwoInvariantMassesHist : public HistogramPrototype2D
{
  public:
  TwoInvariantMassesHist(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY, std::shared_ptr<Selector> selector = nullptr);
  protected:
    std::vector<std::pair<double, double>> value2D() const override;

private:
  std::shared_ptr<Selector> leptonSelector;
};

#endif
