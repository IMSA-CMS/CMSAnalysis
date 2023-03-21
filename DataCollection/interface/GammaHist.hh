#ifndef GAMMAHIST_HH
#define GAMMAHIST_HH

#include "HistogramPrototype1D.hh"

class LeptonJetReconstructionModule;

class GammaHist : public HistogramPrototype1D
{
  public:
  GammaHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};

#endif 