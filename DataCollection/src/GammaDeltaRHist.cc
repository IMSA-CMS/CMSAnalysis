#include "CMSAnalysis/DataCollection/interface/GammaDeltaRHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

#include <vector>

GammaDeltaRHist::GammaDeltaRHist(std::shared_ptr<LeptonJetReconstructionModule> iLeptonJetRecoModule, const std::string& iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  leptonJetRecoModule(iLeptonJetRecoModule) 
{
}

std::vector<double> GammaDeltaRHist::value() const
{
  const std::vector<LeptonJet>& LeptonJets = leptonJetRecoModule->getLeptonJets();

  std::vector<double> energy_leptonJets;
  std::vector<double> momentum_leptonJets;
  std::vector<double> gamma_leptonJets; 

  if (LeptonJets.size() > 0)
  {
      for (uint i = 0; i < LeptonJets.size(); i++)
      {
        energy_leptonJets.push_back(LeptonJets[i].getEnergy());
        momentum_leptonJets.push_back(LeptonJets[i].getPt());
      }
      for (uint j = 0; j < LeptonJets.size(); j++)
      {
        gamma_leptonJets.push_back(energy_leptonJets[j] / sqrt(energy_leptonJets[j]*energy_leptonJets[j]-momentum_leptonJets[j]*momentum_leptonJets[j]));
      }
      return {gamma_leptonJets};
  }
  else
  {
      return {-1};
  }  
}