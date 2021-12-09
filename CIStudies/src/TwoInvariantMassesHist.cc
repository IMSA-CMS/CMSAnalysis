#include "CIAnalysis/CIStudies/interface/TwoInvariantMassesHist.hh"
#include "CIAnalysis/CIStudies/interface/Selector.hh"

TwoInvariantMassesHist::TwoInvariantMassesHist(const std::string& iname, int iNBinsX, int iNBinsY, double iMinX, double iMinY, double iMaxX, double iMaxY, std::shared_ptr<Selector> selector) :
  HistogramPrototype2D(iname, iNBinsX, iNBinsY, iMinX, iMinY, iMaxX, iMaxY),
  leptonSelector(selector)
{
}

std::vector<std::pair<double, double>> TwoInvariantMassesHist::value2D() const
{
  auto invMasses = getInput()->getLeptons(InputModule::RecoLevel::Reco, leptonSelector).calculateSameSignInvariantMasses();

  if (invMasses.size() != 2)
  {
    throw std::runtime_error("Incorrect number of invariant masses calculated!");
  }

  return {{invMasses[0], invMasses[1]}};
}
