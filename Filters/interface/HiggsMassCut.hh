#ifndef HIGGSMASSCUT_HH
#define HIGGSMASSCUT_HH

#include "Cut.hh"

class HiggsMassCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event) const override;
};

#endif