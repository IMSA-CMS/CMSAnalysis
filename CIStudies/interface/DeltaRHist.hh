#ifndef DELTARHIST_HH
#define DELTARHIST_HH

#include "HistogramPrototype.hh"

class LeptonJetReconstructionModule;

class DeltaRHist : public HistogramPrototype
{
  public:
  DeltaRHist(const LeptonJetReconstructionModule& iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  double value() const override;

  private:
  const LeptonJetReconstructionModule& leptonJetRecoModule;
};


#endif