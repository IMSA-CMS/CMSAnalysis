#ifndef BLEPTONCUT_HH
#define BLEPTONCUT_HH

#include "Cut.hh"

class BJetCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
};

#endif