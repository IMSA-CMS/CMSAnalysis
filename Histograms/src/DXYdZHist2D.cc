#include "CMSAnalysis/Histograms/interface/DXYdZHist2D.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

#include <vector>

DXYdZHist2D::DXYdZHist2D(const std::string &iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY) : 
    HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY)
    {} 

std::vector<std::pair<double, double>> DXYdZHist2D::value2D() const

{
  auto particles = getInput()->getLeptons(EventInput::RecoLevel::Reco);

  std::vector<std::pair<double, double>> dXYdZpairs;
  
  for (auto p : particles)
  {
    dXYdZpairs.push_back({{p.getInfo("dxy")}, {p.getInfo("dz")}});
  }

  return dXYdZpairs;
};