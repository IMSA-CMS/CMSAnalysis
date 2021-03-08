#include "CIAnalysis/CIStudies/interface/MassResolutionHist.hh"

#include "CIAnalysis/CIStudies/interface/ResolutionPrototype.hh"
#include "CIAnalysis/CIStudies/interface/GenSimIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/RecoIdentificationModule.hh"
#include "CIAnalysis/CIStudies/interface/HistogramPrototype.hh"
//#include "CIAnalysis/CIStudies/interface/ParticleCollection.hh"

double MassResolutionHist::getValue(const ParticleCollection& partColl) const
{
  return partColl.getInvariantMass();
}
