#ifndef GAMMADELTARHIST2D_HH
#define GAMMADELTARHIST2D_HH

#include "CMSAnalysis/DataCollection/interface/HistogramPrototype2D.hh"

class LeptonJetReconstructionModule;

class GammaDeltaRHist2D : public HistogramPrototype2D
{
  public:
  GammaDeltaRHist2D(std::shared_ptr<LeptonJetReconstructionModule> ileptonJetRecoModule, const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY);
  protected:
    std::vector<std::pair<double, double>> value2D() const override;
  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};

#endif