#ifndef DARKPHOTONHIGHMASSCUT_HH
#define DARKPHOTONHIGHMASSCUT_HH

#include "Cut.hh"

class DarkPhotonHighMassCut : public Cut
{
    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
	private:
		const double minJetMass = 10;
};

#endif