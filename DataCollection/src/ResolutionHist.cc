#include "CMSAnalysis/DataCollection/interface/ResolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include <iostream>



ResolutionHist::ResolutionHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum):
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    leptonMatchMod(iLeptonJetMatchingModule)
{

}

std::vector<double> ResolutionHist::value() const
{ 
  //pair of particles <gen sim, reco>
  const auto matchedPairs = leptonMatchMod->getMatchingPairs();
  std::vector<double> ResolutionVector{};
  std::vector<std::pair<double, double>> gammaVector{};
  for (auto matchedPair : matchedPairs)
  { 
    gammaVector.push_back(std::get<0>(matchedPair).getGamma(), std::get<1>(matchedPair).getGamma());    
  }
  for (auto gammaPair : gammaVector)
  {
    auto resolution = (gammaPair.first()-gammaPair.second())/(gammaPair.first());
    ResolutionVector.push_back(resolution);
  }
  return ResolutionVector;
  }
