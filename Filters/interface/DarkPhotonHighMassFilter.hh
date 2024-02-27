#ifndef DARKPHOTONHIGHMASSFILTER_HH
#define DARKPHOTONHIGHMASSFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"

class DarkPhotonHighMassFilter : public Filter
{
    public:
		DarkPhotonHighMassFilter(double minJetMassi): minJetMass(minJetMassi) {}
	protected:
        std::string getFilterString(const EventInput* input) const override;
	private:
		const double minJetMass;
};

#endif