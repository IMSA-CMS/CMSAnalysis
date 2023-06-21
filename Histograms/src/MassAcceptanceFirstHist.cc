#include "CMSAnalysis/Histograms/interface/MassAcceptanceFirstHist.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"
#include <vector>

MassAcceptanceFirstHist::MassAcceptanceFirstHist(const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
{
}

std::vector<double> MassAcceptanceFirstHist::value() const
{
  const auto leptonJets = getInput()->getLeptons(EventInput::RecoLevel::GenSim);
  return {leptonJets.getInvariantMass()};
}
