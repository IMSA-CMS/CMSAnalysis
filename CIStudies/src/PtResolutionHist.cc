#include "CIAnalysis/CIStudies/interface/PtResolutionHist.hh"

#include "CIAnalysis/CIStudies/interface/ResolutionPrototype.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"
//#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"

double PtResolutionHist::getValue(const ParticleCollection& partColl) const
{
  //std::cerr << "congrats you made it" << std::endl;
  return partColl.getLeadingTransverseMomentum();
}
