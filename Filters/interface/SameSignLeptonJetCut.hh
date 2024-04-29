#ifndef SAMESIGNLEPTONJETCUT_HH
#define SAMESIGNLEPTONJETCUT_HH

#include "Cut.hh"

class SameSignLeptonJetCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
	private:
};

#endif