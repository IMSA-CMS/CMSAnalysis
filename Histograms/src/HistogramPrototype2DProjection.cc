#include "CMSAnalysis/Histograms/interface/HistogramPrototype2DProjection.hh"
#include "CMSAnalysis/Filters/interface/Selector.hh"

HistogramPrototype2DProjection::HistogramPrototype2DProjection(const std::string& iname, std::shared_ptr<HistogramPrototype2D> i2DHist, bool iXProjection, int iNBins, double iminimum, double imaximum) : 
  HistogramPrototype1D(
    iname,
    iNBins == -1 ? (iXProjection ? i2DHist->getNBinsX() : i2DHist->getNBinsY()) : iNBins,
    iminimum == -1 ? (iXProjection ? i2DHist->getMinX() : i2DHist->getMinY()) : iminimum,
    imaximum == -1 ? (iXProjection ? i2DHist->getMaxX() : i2DHist->getMaxY()) : imaximum
  )
{
  hist2D = i2DHist;
  xProjection = iXProjection;
}

std::vector<double> HistogramPrototype2DProjection::value() const
{
  auto value = hist2D.get()->value();

  std::vector<double> vec;  

  for (int i = 0; i < int(value.size()) - 1; i += 2)
  {
    vec.push_back(xProjection ? value[i] : value[i + 1]);
  }

  return vec;

}

// std::vector<double> HistogramPrototype2DProjection::protectedValue(EventInput::RecoLevel typeGenSim) const
// {
//   if (multipleMasses)
//   {
//     auto particles = getInput()->getLeptons(typeGenSim);
//     // std::cout << "2" << recoParticles.getNumParticles() << std::endl;
//     auto inv = particles.calculateSameSignInvariantMasses(true);
//     return {inv};
//   } 
//   else 
//   {
//     auto particles = getInput()->getLeptons(typeGenSim);
//     // std::cout << particles.getNumParticles() << std::endl;
//     // std::cout << "4-" << particles.calculateSameSignInvariantMass() << std::endl;
//     auto inv = particles.calculateSameSignInvariantMass();
//     // std::cout << inv << std::endl;
//     return {inv};
//   }
// }
