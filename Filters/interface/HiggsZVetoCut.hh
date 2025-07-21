#ifndef HIGGSZVETOCUT_HH
#define HIGGSZVETOCUT_HH

#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"

class HiggsZVetoCut : public Cut
{
  protected:
    bool checkEventInternal(const Event &event, const EventInput *input) const override;
};

#endif