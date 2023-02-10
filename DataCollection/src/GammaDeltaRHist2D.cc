#include "CMSAnalysis/DataCollection/interface/GammaDeltaRHist2D.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"

#include <vector>

GammaDeltaRHist2D::GammaDeltaRHist2D(const std::string &iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY) : 
    HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY){};

std::vector<std::pair<double, double>> GammaDeltaRHist2D::value2D() const

{
  std::vector<std::pair<double, double>> error = {{-1,-1}}; 
  std::vector<std::pair<double, double>> gammaDeltaRvalues;

  std::vector<double> gamma_leptonJets; 

  const std::vector<LeptonJet>& LeptonJets = leptonJetRecoModule->getLeptonJets();
  
  if (LeptonJets.size() > 0)
  {
      for (uint i = 0; i < LeptonJets.size(); i++)
      {
        gamma_leptonJets.push_back(LeptonJets[i].getGamma());
      }
  }
  else    
  {
      return error;
  } 

  const std::vector<double>& deltaRValues = leptonJetRecoModule->getDeltaRValues();

  if (deltaRValues.size() > 0)
  {
      for (uint j = 0; j < gamma_leptonJets.size(); j++)
      {
        //gammaDeltaRvalues.push_back({{gamma_leptonJets[j]}, {deltaRValues[0][j]}})
        gammaDeltaRvalues.push_back({{gamma_leptonJets[j]}, {deltaRValues[j]}});
      }
  }
  else
  {
      return error;
  }
  //std::vector<std::pair<double, double>> gammaDeltaRvalues = {{gamma_leptonJets}, {deltaRValues[0]}};
  //std::vector<std::vector<double>> gammaDeltaRvalues = {{gamma_leptonJets}, {deltaRValues[0]}};
  
  return gammaDeltaRvalues;
};