#include "CIAnalysis/CIStudies/interface/TotalEventsHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include <vector>

TotalEventsHist::TotalEventsHist(const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum)
 //genSimIdentification(iGenSimIdentificationModule)
{
}

std::vector<double> TotalEventsHist::value() const
{
  const ParticleCollection leptonJets = getInput()->getParticles(InputModule::RecoLevel::GenSim);
  return {leptonJets.getInvariantMass()};
}
