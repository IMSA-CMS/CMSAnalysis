#include "CIAnalysis/CIStudies/interface/MassAcceptanceFirstHist.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJet.hh"
#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"
#include <vector>

MassAcceptanceFirstHist::MassAcceptanceFirstHist(const std::string iname, int iNBins, double iminimum, double imaximum) :
  HistogramPrototype(iname, iNBins, iminimum, imaximum)
{
}

std::vector<double> MassAcceptanceFirstHist::value() const
{
  const ParticleCollection leptonJets = getInput()->getParticles(InputModule::RecoLevel::GenSim);
  return {leptonJets.getInvariantMass()};
}
