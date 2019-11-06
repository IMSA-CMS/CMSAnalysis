#include "CIAnalysis/CIStudies/interface/CollinsSoperFilter.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"

CollinsSoperFilter::CollinsSoperFilter(const GenSimIdentificationModule& genSimModule, double hiCut) :
  genSim(genSimModule),
  highCutoff(hiCut)
{}

std::string CollinsSoperFilter::makeFilterString(const edm::EventBase& event)
{
  double collinsSoper = genSim.getGenParticles().getCollinsSoper();

  if (abs(collinsSoper) > highCutoff)
    return "HighCS";
  else
    return "";
}
