#ifndef HIGGSCUT_HH
#define HIGGSCUT_HH

#include "Cut.hh"

class HiggsCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event) const override;
};

#endif