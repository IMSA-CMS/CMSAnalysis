#include "CMSAnalysis/Histograms/interface/GammaDeltaRHist2D.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

#include <vector>

GammaDeltaRHist2D::GammaDeltaRHist2D(std::shared_ptr<LeptonJetReconstructionModule> ileptonJetRecoModule, const std::string &iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY) : 
    HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY),
    leptonJetRecoModule(ileptonJetRecoModule)
    {} 

std::vector<std::pair<double, double>> GammaDeltaRHist2D::value2D() const

{
  auto LeptonJets = leptonJetRecoModule->getLeptonJets();
  
  //std::cout<< "got leptons" << "\n";

  std::vector<std::pair<double, double>> gammaDeltaRpairs;

  for (LeptonJet jet : LeptonJets)
  {
    const std::vector<Particle>& particles = jet.getParticles();

    gammaDeltaRpairs.push_back({{jet.getGamma()}, {particles[0].getDeltaR(particles[1])}});
  }
  return gammaDeltaRpairs;
};