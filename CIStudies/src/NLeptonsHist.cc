#include "CIAnalysis/CIStudies/interface/MatchingModule.hh"
#include "CIAnalysis/CIStudies/interface/NLeptonsHist.hh"

NLeptonsHist::NLeptonsHist(const std::shared_ptr<MatchingModule> imatchModule, const std::string& iname, int iNBins, double iminimum, double imaximum, int itargetPdgId):
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  matchModule(imatchModule),
  targetPdgId(itargetPdgId)
{
}

std::vector<double> NLeptonsHist::value() const
{
  const MatchingPairCollection& pairs = matchModule->getMatchingBestPairs();
  //std::cout << "TargetParticle: " << targetPdgId << " Number of Matched Particles: " << pairs.getSize() << "\n";

  if(targetPdgId == 0)
    {
      // std::cout << "Particles Found: " << pairs.getSize() << '\n';
      return {pairs.getSize()};
    }
  else
    {
      int count = 0;
      for(const auto& p : pairs.getPairs())
	{
	  //std::cout << p.getRecoParticle().pdgId() << " ";
	  if(abs(p.getRecoParticle().pdgId()) == targetPdgId)
	    {
	      count++;
	    }
	}
      // std::cout << "\nParticles Found: " << count << std::endl;
      return {static_cast<double>(count)};
    }
}
