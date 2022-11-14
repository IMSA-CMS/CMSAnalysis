#include "CMSAnalysis/DataCollection/interface/MassAcceptanceFirstHist.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJet.hh"
#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"
#include "CMSAnalysis/DataCollection/interface/Lepton.hh"
#include "CMSAnalysis/DataCollection/interface/InputModule.hh"
#include <vector>

MassAcceptanceFirstHist::MassAcceptanceFirstHist(const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
{
}

std::vector<double> MassAcceptanceFirstHist::value() const
{
  const auto leptonJets = getInput()->getLeptons(InputModule::RecoLevel::GenSim);
  return {leptonJets.getInvariantMass()};
}
