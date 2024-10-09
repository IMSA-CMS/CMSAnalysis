#ifndef HIGGSTRIGGERCUT_HH
#define HIGGSTRIGGERCUT_HH

#include <string>
#include <map>

#include "Cut.hh"

class HiggsTriggerCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
};

#endif