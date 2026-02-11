#include "CMSAnalysis/Histograms/interface/MassEta2Dhist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh" 

#include <vector>

MassEta2Dhist::MassEta2Dhist(const std::string &iname, int iNBinsX, int iNBinsY, 
                             double iMinX, double iMinY, double iMaxX, double iMaxY) : 
    HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY)
{} 


std::vector<std::pair<double, double>> MassEta2Dhist::value2D() const
{
  auto particles = getInput()->getSpecial("leptonJet");

  std::vector<std::pair<double, double>> massEtaPairs(particles.size());

  int index = 0;
  for (auto p : particles)
  {
    // x = mass   y = eta
    massEtaPairs[index] =
      std::make_pair(p.getMass(), p.getEta());
    index++;
  }

  massEtaPairs.resize(index);
  return massEtaPairs;
}



