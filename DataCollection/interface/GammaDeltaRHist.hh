#ifndef GAMMADELTARHIST_HH
#define GAMMADELTARHIST_HH

#include "HistogramPrototype1D.hh"

class LeptonJetReconstructionModule;

class GammaDeltaRHist : public HistogramPrototype1D
{
  public:
  GammaDeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};

#endif