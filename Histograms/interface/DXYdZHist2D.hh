#ifndef DXYDZHIST2D_HH
#define DXYDZHIST2D_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype2D.hh"

class LeptonJetReconstructionModule;

class DXYdZHist2D : public HistogramPrototype2D
{
  public:
  DXYdZHist2D(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY);
  protected:
    std::vector<std::pair<double, double>> value2D() const override;
};

#endif