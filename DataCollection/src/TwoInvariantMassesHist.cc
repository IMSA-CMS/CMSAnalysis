#include "CMSAnalysis/DataCollection/interface/TwoInvariantMassesHist.hh"
#include "CMSAnalysis/DataCollection/interface/Selector.hh"

TwoInvariantMassesHist::TwoInvariantMassesHist(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY) :
  HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY)
{
}

std::vector<std::pair<double, double>> TwoInvariantMassesHist::value2D() const
{
  auto invMasses = getInput()->getLeptons(InputModule::RecoLevel::Reco).calculateSameSignInvariantMasses();

  if (invMasses.size() != 2)
  {
    throw std::runtime_error("Incorrect number of invariant masses calculated!");
  }

  return {{invMasses[0], invMasses[1]}};
}