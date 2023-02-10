#include "CMSAnalysis/DataCollection/interface/GammaHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

#include <vector>

GammaHist::GammaHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
{
}

std::vector<double> GammaHist::value() const
{
  const std::vector<LeptonJet>& LeptonJets = leptonJetRecoModule->getLeptonJets();

  std::vector<double> gamma_leptonJets; 

  if (LeptonJets.size() > 0)
  {
      for (uint i = 0; i < LeptonJets.size(); i++)
      {
        gamma_leptonJets.push_back(LeptonJets[i].getGamma());
      }
      return {gamma_leptonJets};
  }
  else
  {
      return {-1};
  }  
}