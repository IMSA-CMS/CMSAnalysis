#include "CMSAnalysis/Histograms/interface/ResolutionHist.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include "CMSAnalysis/Utility/interface/GenSimSimpleImplementation.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include <iostream>

ResolutionHist::ResolutionHist(std::shared_ptr<LeptonJetMatchingModule> ileptonJetMatchModule, const std::string& iname, int iNBins, double iminimum, double imaximum):
    HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
    leptonJetMatchModule(ileptonJetMatchModule)
{
} 

std::vector<double> ResolutionHist::value() const 
{ 
  //pair of particles <gen sim, reco>
  const auto& matchedPairs = leptonJetMatchModule->getMatchingPairs();
  std::vector<std::pair<double, double>> gammaVector{};
  std::vector<double> ResolutionVector{};
  for (auto matchedPair : matchedPairs)
  { 
    gammaVector.push_back(std::make_pair(matchedPair.first.getGamma(), matchedPair.second.getGamma()));
    //std::cout << "Gen Sim Gamma: " << matchedPair.first.getGamma() << "Reco Gamma: " << matchedPair.second.getGamma() << "\n";
  }
  for (auto gammaPair : gammaVector)
  {
    auto resolution = (gammaPair.first-gammaPair.second)/(gammaPair.first);
    //std::cout<< "Resolution: " << resolution << "\n";
    ResolutionVector.push_back(resolution);
  }
  return ResolutionVector;
}
 