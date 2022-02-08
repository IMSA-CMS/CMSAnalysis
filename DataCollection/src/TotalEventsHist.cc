#include "CMSAnalysis/DataCollection/interface/TotalEventsHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include <vector>

TotalEventsHist::TotalEventsHist(const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
 //genSimIdentification(iGenSimIdentificationModule)
{
}

std::vector<double> TotalEventsHist::value() const
{
  const ParticleCollection leptonJets = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
  return {leptonJets.getInvariantMass()};
}
