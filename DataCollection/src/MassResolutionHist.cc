#include "CMSAnalysis/DataCollection/interface/MassResolutionHist.hh"

#include "CMSAnalysis/DataCollection/interface/ResolutionPrototype.hh"
#include "CMSAnalysis/DataCollection/interface/HistogramPrototype.hh"
//#include "CMSAnalysis/DataCollection/interface/ParticleCollection.hh"

double MassResolutionHist::getValue(const ParticleCollection& partColl) const
{
  return partColl.getInvariantMass();
}
