#include "CIAnalysis/CIStudies/interface/TotalEventsHist.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include <vector>

TotalEventsHist::TotalEventsHist(const std::shared_ptr<GenSimIdentificationModule> iGenSimIdentificationModule, const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype(iname, iNBins, iminimum, imaximum),
  genSimIdentification(iGenSimIdentificationModule)
{
}

double TotalEventsHist::value() const
{
  const ParticleCollection leptonJets = genSimIdentification->getGenParticles();
  return leptonJets.getInvariantMass();
}
