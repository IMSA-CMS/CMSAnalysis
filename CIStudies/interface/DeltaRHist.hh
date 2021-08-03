#ifndef DELTARHIST_HH
#define DELTARHIST_HH

#include "HistogramPrototype1D.hh"

class LeptonJetReconstructionModule;

class DeltaRHist : public HistogramPrototype1D
{
  public:
  DeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};


#endif
