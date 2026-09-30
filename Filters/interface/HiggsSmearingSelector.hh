#ifndef HIGGSSMEARINGSELECTOR_HH
#define HIGGSSMEARINGSELECTOR_HH

#include "CMSAnalysis/Filters/interface/HiggsSelector.hh"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class HiggsSmearingSelector : public HiggsSelector
{
    public:
		HiggsSmearingSelector(ScaleFactor::SystematicType type);
	private:
		ScaleFactor::SystematicType type;
	protected:
		virtual void adjustEnergy(Particle& particle) const override;

};

#endif