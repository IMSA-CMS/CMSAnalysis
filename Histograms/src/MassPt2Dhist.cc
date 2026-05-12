#include "CMSAnalysis/Histograms/interface/MassPt2Dhist.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh" 

#include <vector>

MassPt2Dhist::MassPt2Dhist(const std::string &iname, int iNBinsX, int iNBinsY, 
                             double iMinX, double iMinY, double iMaxX, double iMaxY) : 
    HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY)
{} 


std::vector<std::pair<double, double>> MassPt2Dhist::value2D() const
{
  auto particles = this->getInput()->getSpecial("leptonJet");

  std::vector<std::pair<double, double>> massPtPairs(particles.size());

  int index = 0;
  for (auto p : particles)
  {
    massPtPairs[index] =
      std::make_pair(p.getMass(), p.getPt());
    index++;
  }

  massPtPairs.resize(index);
  return massPtPairs;
}


