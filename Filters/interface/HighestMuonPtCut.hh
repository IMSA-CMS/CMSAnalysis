#ifndef HIGHESTMUONPTCUT_HH
#define HIGHESTMUONPTCUT_HH

#include "Cut.hh"

class HighestMuonPtCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
};

#endif