#include "CMSAnalysis/Histograms/interface/TotalEventsHist.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/InputModule.hh"
#include <vector>

TotalEventsHist::TotalEventsHist(const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
 //genSimIdentification(iGenSimIdentificationModule)
{
}

std::vector<double> TotalEventsHist::value() const
{
  const auto leptonJets = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
  return {leptonJets.getInvariantMass()};
}
