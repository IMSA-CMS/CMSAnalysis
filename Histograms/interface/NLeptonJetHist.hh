#ifndef NLEPTONJETHIST_HH
#define NLEPTONJETHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"

class LeptonJetReconstructionModule;

class NLeptonJetHist : public HistogramPrototype1D
{
  public:
  NLeptonJetHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};


#endif
