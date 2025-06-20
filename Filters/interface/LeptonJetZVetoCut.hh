#ifndef LEPTONJETZVETOCUT_HH
#define LEPTONJETZVETOCUT_HH

#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

class LeptonJetZVetoCut : public Cut
{
  protected:
    bool checkEventInternal(const Event &event, const EventInput *input) const override;
};

#endif