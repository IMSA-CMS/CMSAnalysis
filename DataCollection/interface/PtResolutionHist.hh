#ifndef PTRESOLUTIONHIST_HH
#define PTRESOLUTIONHIST_HH

#include "CMSAnalysis/DataCollection/interface/ResolutionPrototype.hh"

class PtResolutionHist : public ResolutionPrototype
{
  public:
    using ResolutionPrototype::ResolutionPrototype; //copies constructor over from ResolutionPrototype
  protected:
    double getValue(const ParticleCollection<Lepton>& partColl) const override;
};

#endif
