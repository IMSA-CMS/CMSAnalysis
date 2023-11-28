#ifndef GENSIMDELTARTWOJETSPSEDOFILTEREDHIST_HH
#define GENSIMDELTARTWOJETSPSEDOFILTEREDHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh" 


class GenSimDeltaRTwoJetsPsedoFilteredHist : public HistogramPrototype1D
{
  public:
  GenSimDeltaRTwoJetsPsedoFilteredHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::shared_ptr<LeptonJetReconstructionModule> lepJetModule);
  std::vector<double> value() const override;
  protected:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJets;


};


#endif
