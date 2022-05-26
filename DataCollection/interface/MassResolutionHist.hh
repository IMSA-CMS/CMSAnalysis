#ifndef MASSRESOLUTIONHIST_HH
#define MASSRESOLUTIONHIST_HH

#include "CMSAnalysis/DataCollection/interface/ResolutionPrototype.hh"

class MassResolutionHist : public ResolutionPrototype
{
  public:
    using ResolutionPrototype::ResolutionPrototype; //copies constructor over from ResolutionPrototype
  protected:
    double getValue(const ParticleCollection& partColl) const override;
};

#endif
