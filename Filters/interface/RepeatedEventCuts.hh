#ifndef REPEATEDEVENTCUTS_HH
#define REPEATEDEVENTCUTS_HH

#include "Cut.hh"
#include <unordered_set>
#include "Rtypes.h"


class RepeatedEventCuts : public Cut
{
    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
    private:
        mutable std::unordered_set<ULong64_t> eventIDs;
};

#endif