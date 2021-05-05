#ifndef NLEPTONJETHIST_HH
#define NLEPTONJETHIST_HH

#include "HistogramPrototype.hh"

class LeptonJetReconstructionModule;

class NLeptonJetHist : public HistogramPrototype
{
  public:
  NLeptonJetHist(const LeptonJetReconstructionModule& iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  double value() const override;

  private:
  const LeptonJetReconstructionModule& leptonJetRecoModule;
};


#endif