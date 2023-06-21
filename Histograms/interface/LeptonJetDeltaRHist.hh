#ifndef LEPTONJETDELTARHIST_HH
#define LEPTONJETDELTARHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"

class LeptonJetReconstructionModule;

// To use this histogram module, you need to add to the Plan.hh file the following lines:
// #include "CMSAnalysis/DataCollection/interface/LeptonJetDeltaRHist.hh"
// auto deltaRHist = make_shared<LeptonJetDeltaRHist>(recoMod, "Lepton Jet Delta R", 500, 0, 2);
// histMod->addHistogram(deltaRHist);
//
// This histogram module takes all events with two leptons and finds the difference in the phi-eta plane between them
class LeptonJetDeltaRHist : public HistogramPrototype1D
{
  public:
  LeptonJetDeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum);
  std::vector<double> value() const override;

  private:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJetRecoModule;
};


#endif
