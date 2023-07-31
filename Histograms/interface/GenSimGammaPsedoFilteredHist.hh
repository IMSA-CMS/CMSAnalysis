#ifndef GENSIMGAMMAPSEDOFILTEREDHIST_HH
#define GENSIMGAMMAPSEDOFILTEREDHIST_HH

#include "CMSAnalysis/Utility/interface/HistogramPrototype1D.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh" 


class GenSimGammaPsedoFilteredHist : public HistogramPrototype1D
{
  public:
  GenSimGammaPsedoFilteredHist(const std::string& iname, int iNBins, double iminimum, double imaximum, std::shared_ptr<LeptonJetReconstructionModule> lepJetModule);
  std::vector<double> value() const override;
  protected:
  std::shared_ptr<LeptonJetReconstructionModule> leptonJets;


};


#endif
