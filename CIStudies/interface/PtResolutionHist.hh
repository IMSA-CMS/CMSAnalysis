#ifndef PTRESOLUTIONHIST_HH
#define PTRESOLUTIONHIST_HH

#include "CIAnalysis/CIStudies/interface/ResolutionPrototype.hh"

class PtResolutionHist : public ResolutionPrototype
{
  public:
    using ResolutionPrototype::ResolutionPrototype; //copies constructor over from ResolutionPrototype
  protected:
    double getValue(const ParticleCollection& partColl) const override;
};

#endif
