#ifndef LEPTONJETDELTARHIST_HH
#define LEPTONJETDELTARHIST_HH

#include "HistogramPrototype1D.hh"

class LeptonJetReconstructionModule;

class LeptonJetDeltaRHist : public HistogramPrototype1D
{
  public:
  LeptonJetDeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};


#endif
