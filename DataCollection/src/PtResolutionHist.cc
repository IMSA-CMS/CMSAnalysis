#include "CMSAnalysis/DataCollection/interface/PtResolutionHist.hh"

#include "CMSAnalysis/DataCollection/interface/ResolutionPrototype.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramPrototype.hh"
//#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"

double PtResolutionHist::getValue(const ParticleCollection& partColl) const
{
  //std::cerr << "congrats you made it" << std::endl;
  return partColl.getLeadingTransverseMomentum();
}
