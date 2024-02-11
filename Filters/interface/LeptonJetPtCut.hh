#ifndef LEPTONJETPTCUT_HH
#define LEPTONJETPTCUT_HH

#include "Cut.hh"

class LeptonJetPtCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
};

#endif