#ifndef NLEPTONJETHIST_HH
#define NLEPTONJETHIST_HH

#include "HistogramPrototype.hh"

class LeptonJetReconstructionModule;

class NLeptonJetHist : public HistogramPrototype
{
  public:
  NLeptonJetHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  double value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};


#endif