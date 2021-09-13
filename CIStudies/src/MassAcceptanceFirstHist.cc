#include "CIAnalysis/CIStudies/interface/MassAcceptanceFirstHist.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include <vector>

MassAcceptanceFirstHist::MassAcceptanceFirstHist(const std::shared_ptr<GenSimIdentificationModule> iGenSimIdentificationModule, const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype1D(iname, iNBins, iminimum, imaximum),
  genSimIdentification(iGenSimIdentificationModule)
{
}

std::vector<double> MassAcceptanceFirstHist::value() const
{
  const ParticleCollection leptonJets = genSimIdentification->getGenParticles();
  return {leptonJets.getInvariantMass()};
}
