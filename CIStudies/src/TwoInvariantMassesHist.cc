#include "CIAnalysis/CIStudies/interface/TwoInvariantMassesHist.hh"

#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"

TwoInvariantMassesHist::TwoInvariantMassesHist(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY, const std::shared_ptr<RecoIdentificationModule> iRecoModule) :
  HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY),
  recoModule(iRecoModule)
{
}

std::vector<std::pair<double, double>> TwoInvariantMassesHist::value2D() const
{
  auto invMasses = recoModule->getRecoCandidates().calculateSameSignInvariantMasses();

  if (invMasses.size() != 2)
  {
    throw std::runtime_error("Incorrect number of invariant masses calculated!");
  }

  return {{invMasses[0], invMasses[1]}};
}
