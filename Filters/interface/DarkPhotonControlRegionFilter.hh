#ifndef DARKPHOTONCONTROLREGIONFILTER_HH
#define DARKPHOTONControlRegionFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"

class DarkPhotonControlRegionFilter : public Filter
{
    public:
		DarkPhotonControlRegionFilter(double minJetMassi): minJetMass(minJetMassi) {}
	protected:
        std::string getFilterString(const EventInput* input) const override;
	private:
		const double minJetMass;
};

#endif