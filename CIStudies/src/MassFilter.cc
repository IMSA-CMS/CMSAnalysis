#include "CIAnalysis/CIStudies/interface/MassFilter.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

MassFilter::MassFilter(const GenSimIdentificationModule& genSimModule, double hiCut) :
  genSim(genSimModule),
  highCutoff(hiCut)
{}

std::string MassFilter::makeFilterString(const edm::EventBase& event)
{
  double invariantMass = genSim.getGenParticles().getInvariantMass();

  if (invariantMass > highCutoff)
    return "HighMass";
  else
    return "";
}
