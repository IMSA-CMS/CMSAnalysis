#include "CMSAnalysis/Histograms/interface/GetNthHighestPtHist.hh"

#include "CMSAnalysis/Histograms/interface/GenSimRecoPrototype.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"

GetNthHighestPtHist::GetNthHighestPtHist(EventInput::RecoLevel typeSwitch, const std::string& iname, int iNBins, double iminimum, double imaximum, int inthMuon, const ParticleType& parType) :
  GenSimRecoPrototype(typeSwitch, iname, iNBins, iminimum, imaximum),
  nthMuon(inthMuon),
  particleType(parType)
{}


std::vector<double> GetNthHighestPtHist::protectedValue(EventInput::RecoLevel typeGenSim) const
{
  auto particles = getInput()->getLeptons(typeGenSim);
  if (particleType == ParticleType::none())
  {
    auto pt = particles.getNthHighestPt(nthMuon);
    //std::cerr << "Reco pT: " << recoPt << std::endl;
    if (pt <= 0)
    {
      return {};
    }
    return {pt};
  }
  int count = 0;
  std::sort(particles.begin(), particles.end());
  for (auto particle : particles.getParticles())
  {
    if (particle.getType() == particleType)
    {
      count ++;
      if (count == nthMuon)
      {
        if (particle.getPt() <= 0)
        {
          return {};
        }
        return {particle.getPt()};
      }
    }
  }
  return {};
}
