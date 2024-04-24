#ifndef DARKPHOTONCONTROLREGIONFILTER_HH
#define DARKPHOTONControlRegionFILTER_HH

#include "CMSAnalysis/Utility/interface/Filter.hh"

class DarkPhotonControlRegionFilter : public Filter
{
    public:
		DarkPhotonControlRegionFilter(double minJetMassi, double minDXYi, double minDZi): minJetMass(minJetMassi), minDXY(minDXYi), minDZ(minDZi) {}
	protected:
        std::string getFilterString(const EventInput* input) const override;
	private:
		const double minJetMass;
		const double minDXY;
		const double minDZ;
};

#endif