#ifndef TWOINVARIANTMASSESHIST_HH
#define TWOINVARIANTMASSESHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype2D.hh"

class TwoInvariantMassesHist : public HistogramPrototype2D
{
  public:
  TwoInvariantMassesHist(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY);
  protected:
    std::vector<std::pair<double, double>> value2D() const override;
};

#endif
