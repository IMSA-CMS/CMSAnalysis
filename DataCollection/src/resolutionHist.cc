#include "CMSAnalysis/DataCollection/interface/resolutionHist.hh"
#include "CMSAnalysis/DataCollection/interface/Particle.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimParticle.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimSimpleImplementation.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
#include <iostream>

resolutionHist::resolutionHist(std::shared_ptr<LeptonJetMatchingModule> iLeptonJetMatchingModule, const std::string& iname, int iNBins, double iminimum, double imaximum):
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    LeptonJetMatchingModule(iLeptonJetMatchingModule),
{

}

std::vector<double> resolutionHist::value() const
{
  //Gen Sim section
  const auto matchedPairs = LeptonJetMatchingModule->getMatchingPairs();
  std::vector<double> gammaVector{};
  std::vector<double> deltaRVector{};
  std::vector<double> ResolutionVector{};
  for (auto matchedPair : matchedPairs)
  { 
    if (analysis=="Gamma")
    {
        ResolutionVector.push_back(matchedPair.first().getGamma());

    }
    else 
    {
        std::runtime_error("analysis incorectly inputed");
    }
  }
  
  return gammaVector;
  }
