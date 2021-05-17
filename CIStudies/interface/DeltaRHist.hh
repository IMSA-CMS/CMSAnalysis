#ifndef DELTARHIST_HH
#define DELTARHIST_HH

#include "HistogramPrototype.hh"

class LeptonJetReconstructionModule;

class DeltaRHist : public HistogramPrototype
{
  public:
  DeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  double value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};


#endif