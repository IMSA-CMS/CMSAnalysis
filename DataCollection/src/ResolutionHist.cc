#include "CMSAnalysis/DataCollection/interface/ResolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include <iostream>

ResolutionHist::ResolutionHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum):
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    LeptonJetMatchingModule(iLeptonJetMatchingModule),
{

}

std::vector<double> ResolutionHist::value() const
{ 
  //pair of particles <gen sim, reco>
  const auto matchedPairs = LeptonJetMatchingModule->getMatchingPairs();
  std::vector<double> gammaVector{};
  std::vector<std::pair<double, double>> ResolutionVector{};
  for (auto matchedPair : matchedPairs)
  { 
    if (analysis=="Gamma")
    {
        gammaVector.push_back(matchedPair.first().getGamma(), matchedPair.second().getGamma());
    }
    else 
    {
        std::runtime_error("analysis incorectly inputed");
    }
  }
  for (auto gammaPair : gammaVector)
  {
    auto resolution = (gammaPair.first()-gammaPair.second())/(gammaPair.first());
    ResolutionVector.push_back(resolution);
  }
  return ResolutionVector;
  }
