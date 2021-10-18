#ifndef LEPTONJETPTHIST_HH
#define LEPTONJETPTHIST_HH

#include "HistogramPrototype.hh"

class LeptonJetReconstructionModule;

class LeptonJetPtHist : public HistogramPrototype
{
  public:
  LeptonJetPtHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};


#endif